#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/all_dijkstra.h"
#include "../algorithm/grasp.h"
#include "../algorithm/range_dijkstra.h"
#include "../algorithm/range_gs_phast.h"
#include "../algorithm/range_gs_rphast.h"
#include "../algorithm/range_phast.h"
#include "../algorithm/range_rphast.h"
#include "../algorithm/rphast.h"
#include "../algorithm/rphast_preprocess.h"
#include "../algorithm/util.h"
#include "../graph/graph.h"
#include "../util.h"

// 2sfca using range-dijkstra
std::vector<float> calcDijkstra2SFCA(IGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                     int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
    }

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcRangeDijkstra(g, s_id, flags, max_range);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            if (!d_flag.visited) {
                continue;
            }
            float distance_decay = 1 - d_flag.dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            if (!d_flag.visited) {
                continue;
            }
            float distance_decay = 1 - d_flag.dist / (float)max_range;
            access[i] += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}

// 2sfca using tiled-graph only descending into active cells
std::vector<float> calcTiled2SFCA(ITiledGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                  int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    std::vector<bool> active_tiles(g->tileCount() + 2);
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            short tile = g->getNodeTile(id);
            active_tiles[tile] = true;
        }
    }

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    std::vector<bool> visited(g->nodeCount());
    std::vector<int> dist(g->nodeCount());
    std::mutex m;
#pragma omp parallel for firstprivate(visited, dist)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];
        short s_tile = g->getNodeTile(s_id);

        // init routing components
        auto explorer = g->getGraphExplorer();
        std::priority_queue<pq_item> heap;

        // clear for routing
        heap.push({s_id, 0});
        for (int i = 0; i < visited.size(); i++) {
            visited[i] = false;
            dist[i] = 1000000000;
        }
        dist[s_id] = 0;

        // routing loop
        while (true) {
            if (heap.empty()) {
                break;
            }
            auto item = heap.top();
            int curr_id = item.node;
            heap.pop();
            if (visited[curr_id]) {
                continue;
            }
            visited[curr_id] = true;
            short curr_tile = g->getNodeTile(curr_id);
            auto handler = [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
                int other_id = ref.other_id;
                if (visited[other_id]) {
                    return;
                }
                int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
                if (new_length > max_range) {
                    return;
                }
                if (dist[other_id] > new_length) {
                    dist[other_id] = new_length;
                    heap.push({other_id, new_length});
                }
            };
            if (curr_tile == s_tile || active_tiles[curr_tile]) {
                explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, handler);
            } else {
                explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_SKIP, handler);
            }
        }

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            if (!visited[d_node]) {
                continue;
            }
            float distance_decay = 1 - dist[d_node] / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            if (!visited[d_node]) {
                continue;
            }
            float distance_decay = 1 - dist[d_node] / (float)max_range;
            m.lock();
            access[i] += R * distance_decay;
            m.unlock();
        }
    }
    return access;
}

// 2sfca using combination of reGRASP and isoGRASP
std::vector<float> calcGRASP2SFCA(ITiledGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                  int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    short tilecount = g->tileCount() + 2;
    std::vector<bool> active_tiles(tilecount);
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            short tile = g->getNodeTile(id);
            active_tiles[tile] = true;
        }
    }

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});
    std::vector<bool> found_tiles(tilecount);
    std::mutex m;
#pragma omp parallel for firstprivate(flags, found_tiles)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        for (int i = 0; i < tilecount; i++) {
            found_tiles[i] = false;
        }
        flags.soft_reset();
        calcGRASP(g, s_id, flags, max_range, active_tiles, found_tiles);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            if (!d_flag.visited) {
                continue;
            }
            float distance_decay = 1 - d_flag.dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            if (!d_flag.visited) {
                continue;
            }
            float distance_decay = 1 - d_flag.dist / (float)max_range;
            access[i] += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}

// 2sfca using range-PHAST
std::vector<float> calcRangePHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                       int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
    }

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcRangePHAST(g, s_id, flags, max_range);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist == 1000000000) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist == 1000000000) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            m.lock();
            access[i] += R * distance_decay;
            m.unlock();
        }
    }
    return access;
}

// rphast using queue
std::vector<float> calcRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                   int max_range)
{
    // get closest node for every demand point
    std::queue<int> node_queue;
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push(id);
        }
    }

    // select graph subset
    auto down_edges_subset = preprocessRPHAST(g, std::move(node_queue));

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcRPHAST(g, s_id, flags, down_edges_subset);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            access[i] += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}

// range rphast using queue
std::vector<float> calcRangeRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                        int max_range)
{
    // get closest node for every demand point
    std::queue<int> node_queue;
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push(id);
        }
    }

    // select graph subset
    auto down_edges_subset = preprocessRPHAST(g, std::move(node_queue));

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcRangeRPHAST(g, s_id, flags, max_range, down_edges_subset);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            m.lock();
            access[i] += R * distance_decay;
            m.unlock();
        }
    }
    return access;
}

// range rphast using priority queue
std::vector<float> calcRangeRPHAST2SFCA2(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                         int max_range)
{
    // get closest node for every demand point
    std::priority_queue<pq_item> node_queue;
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push({id, 0});
        }
    }

    // select graph subset
    auto down_edges_subset = preprocessRangeRPHAST(g, std::move(node_queue), max_range);

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcRangeRPHAST(g, s_id, flags, max_range, down_edges_subset);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist > max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist > max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            access[i] += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}

// range-rphast+gs using queue
std::vector<float> calcGSRPHAST2SFCA(CHGraph2* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                     int max_range)
{
    // get closest node for every demand point
    std::queue<int> node_queue;
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    short tilecount = g->tileCount() + 2;
    std::vector<bool> active_tiles(tilecount);
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push(id);
            short tile = g->getNodeTile(id);
            active_tiles[tile] = true;
        }
    }

    // select graph subset
    auto down_edges_subset = preprocessGSRPHAST(g, std::move(node_queue));

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});
    std::vector<bool> found_tiles(tilecount);
    std::mutex m;
#pragma omp parallel for firstprivate(flags, found_tiles)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        for (int i = 0; i < tilecount; i++) {
            found_tiles[i] = false;
        }
        flags.soft_reset();
        calcGSRPHAST(g, s_id, flags, max_range, down_edges_subset, active_tiles, found_tiles);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            access[i] += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}

std::vector<int> calcDijkstraRangeQuery(IGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    for (int i = 0; i < target_points.size(); i++) {
        target_nodes[i] = index.getClosestNode(target_points[i]);
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});

    // compute dist
    calcRangeDijkstra(g, start_node, flags, max_range);
    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}

std::vector<int> calcRangePHASTRangeQuery(ICHGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    for (int i = 0; i < target_points.size(); i++) {
        target_nodes[i] = index.getClosestNode(target_points[i]);
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});

    // compute dist
    calcRangePHAST(g, start_node, flags, max_range);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}

std::vector<int> calcRPHASTRangeQuery(ICHGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    std::queue<int> node_queue;
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            node_queue.push(node);
        }
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});

    auto down_edges_subset = preprocessRPHAST(g, std::move(node_queue));

    // compute dist
    calcRangeRPHAST(g, start_node, flags, max_range, down_edges_subset);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}

std::vector<int> calcRangeRPHASTRangeQuery(ICHGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    std::priority_queue<pq_item> node_queue;
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            node_queue.push({node, 0});
        }
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});

    auto down_edges_subset = preprocessRangeRPHAST(g, std::move(node_queue), max_range);

    // compute dist
    calcRangeRPHAST(g, start_node, flags, max_range, down_edges_subset);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}

std::vector<int> calcGSPHASTRangeQuery(CHGraph2* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    int tile_count = g->tileCount();
    std::vector<bool> found_tiles(tile_count);
    std::vector<bool> active_tiles(tile_count);
    for (int i = 0; i < tile_count; i++) {
        found_tiles[i] = false;
        active_tiles[i] = false;
    }
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            short tile = g->getNodeTile(node);
            active_tiles[tile] = true;
        }
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});

    // compute dist
    calcGSPHAST(g, start_node, flags, max_range, active_tiles, found_tiles);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}

std::vector<int> calcGSRPHASTRangeQuery(CHGraph2* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    int tile_count = g->tileCount();
    std::vector<bool> found_tiles(tile_count);
    std::vector<bool> active_tiles(tile_count);
    for (int i = 0; i < tile_count; i++) {
        found_tiles[i] = false;
        active_tiles[i] = false;
    }
    std::queue<int> node_queue;
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            node_queue.push(node);
            short tile = g->getNodeTile(node);
            active_tiles[tile] = true;
        }
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});

    std::vector<CHEdge4> subset = preprocessGSRPHAST(g, std::move(node_queue));

    // compute dist
    calcGSRPHAST(g, start_node, flags, max_range, subset, active_tiles, found_tiles);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}

std::vector<int> calcGRASPRangeQuery(ITiledGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    int tile_count = g->tileCount();
    std::vector<bool> found_tiles(tile_count);
    std::vector<bool> active_tiles(tile_count);
    for (int i = 0; i < tile_count; i++) {
        found_tiles[i] = false;
        active_tiles[i] = false;
    }
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            short tile = g->getNodeTile(node);
            active_tiles[tile] = true;
        }
    }

    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});

    // compute dist
    calcGRASP(g, start_node, flags, max_range, active_tiles, found_tiles);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}
