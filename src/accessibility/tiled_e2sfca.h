#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/pq_item.h"
#include "../graph/graph.h"

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

std::vector<float> calcTiled2SFCA2(ITiledGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
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

    std::vector<bool> visited(g->nodeCount());
    std::vector<int> dist(g->nodeCount());
    std::vector<bool> found_tiles(tilecount);
    std::mutex m;
#pragma omp parallel for firstprivate(visited, dist, found_tiles)
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
        for (int i = 0; i < tilecount; i++) {
            found_tiles[i] = false;
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
            if (curr_tile == s_tile) {
                explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, handler);
            } else {
                found_tiles[curr_tile] = true;
                explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_SKIP, handler);
            }
        }
        for (int i = 0; i < tilecount; i++) {
            if (!active_tiles[i] || !found_tiles[i]) {
                continue;
            }
            auto& down_edges = g->getIndexEdges(i, Direction::FORWARD);
            for (int j = 0; j < down_edges.size(); j++) {
                TiledSHEdge edge = down_edges[j];
                int curr_len = dist[edge.from];
                int new_len = curr_len + edge.weight;
                if (dist[edge.to] > new_len) {
                    dist[edge.to] = new_len;
                }
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