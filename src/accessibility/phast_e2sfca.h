#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/pq_item.h"
#include "../graph/graph.h"

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
            explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
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
            });
        }
        const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
        int length = down_edges.size();
        for (int i = 0; i < length; i++) {
            auto edge = down_edges[i];
            int curr_len = dist[edge.from];
            int new_len = curr_len + edge.weight;
            if (new_len > max_range) {
                continue;
            }
            if (dist[edge.to] > new_len) {
                dist[edge.to] = new_len;
            }
        }

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            int d_dist = dist[d_node];
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
            int d_dist = dist[d_node];
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

    // select graph subset by marking visited nodes
    auto explorer = g->getGraphExplorer();
    std::vector<bool> graph_subset(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        graph_subset[i] = false;
    }
    while (true) {
        if (node_queue.size() == 0) {
            break;
        }
        auto node = node_queue.front();
        node_queue.pop();
        if (graph_subset[node]) {
            continue;
        }
        graph_subset[node] = true;
        short node_level = g->getNodeLevel(node);
        explorer->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            node_queue.push(ref.other_id);
        });
    }
    // selecting subset of downward edges for linear sweep
    std::vector<CHEdge> down_edges_subset;
    down_edges_subset.reserve(dem_points.size());
    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    for (int i = 0; i < down_edges.size(); i++) {
        auto edge = down_edges[i];
        if (!graph_subset[edge.from]) {
            continue;
        }
        down_edges_subset.push_back(edge);
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
            explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
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
            });
        }
        int length = down_edges_subset.size();
        for (int i = 0; i < length; i++) {
            auto edge = down_edges_subset[i];
            int curr_len = dist[edge.from];
            if (curr_len > max_range) {
                continue;
            }
            int new_len = curr_len + edge.weight;
            if (dist[edge.to] > new_len) {
                dist[edge.to] = new_len;
            }
        }

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            int d_dist = dist[d_node];
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
            int d_dist = dist[d_node];
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

std::vector<float> calcRangeRPHAST2SFCA2(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                         int max_range)
{
    // get closest node for every demand point
    std::priority_queue<pq_item> node_queue;
    std::vector<bool> graph_subset(g->nodeCount());
    std::vector<int> lengths(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        graph_subset[i] = false;
        lengths[i] = 100000000;
    }

    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push({id, 0});
            lengths[id] = 0;
        }
    }

    // select graph subset by marking visited nodes
    auto explorer = g->getGraphExplorer();
    while (true) {
        if (node_queue.empty()) {
            break;
        }
        auto item = node_queue.top();
        int node = item.node;
        node_queue.pop();
        if (graph_subset[node]) {
            continue;
        }
        graph_subset[node] = true;
        int node_len = lengths[node];
        explorer->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &explorer, node_len, &lengths, max_range, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            int new_len = node_len + explorer->getEdgeWeight(ref);
            if (new_len > max_range) {
                return;
            }
            if (new_len < lengths[ref.other_id]) {
                lengths[ref.other_id] = new_len;
                node_queue.push({ref.other_id, new_len});
            }
        });
    }
    // selecting subset of downward edges for linear sweep
    std::vector<CHEdge> down_edges_subset;
    down_edges_subset.reserve(dem_points.size());
    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    for (int i = 0; i < down_edges.size(); i++) {
        auto edge = down_edges[i];
        if (!graph_subset[edge.from]) {
            continue;
        }
        down_edges_subset.push_back(edge);
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
            explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
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
            });
        }
        int length = down_edges_subset.size();
        for (int i = 0; i < length; i++) {
            auto edge = down_edges_subset[i];
            int curr_len = dist[edge.from];
            if (curr_len > max_range) {
                continue;
            }
            int new_len = curr_len + edge.weight;
            if (dist[edge.to] > new_len) {
                dist[edge.to] = new_len;
            }
        }

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            int d_dist = dist[d_node];
            if (d_dist > max_range) {
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
            int d_dist = dist[d_node];
            if (d_dist > max_range) {
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

// computes the R_j value of the supply point during the down-sweep
std::vector<float> calcRangeRPHAST2SFCA3(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                         int max_range)
{
    // get closest node for every demand point
    std::priority_queue<pq_item> node_queue;
    std::vector<bool> graph_subset(g->nodeCount());
    std::vector<int> lengths(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        graph_subset[i] = false;
        lengths[i] = 100000000;
    }

    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    std::vector<int> is_demand(g->nodeCount());
    for (int i = 0; i < is_demand.size(); i++) {
        is_demand[i] = -1;
    }
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push({id, 0});
            is_demand[id] = dem_weights[i];
            lengths[id] = 0;
        }
    }

    // select graph subset by marking visited nodes
    auto explorer = g->getGraphExplorer();
    while (true) {
        if (node_queue.empty()) {
            break;
        }
        auto item = node_queue.top();
        int node = item.node;
        node_queue.pop();
        if (graph_subset[node]) {
            continue;
        }
        graph_subset[node] = true;
        int node_len = lengths[node];
        explorer->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &explorer, node_len, &lengths, max_range, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            int new_len = node_len + explorer->getEdgeWeight(ref);
            if (new_len > max_range) {
                return;
            }
            if (new_len < lengths[ref.other_id]) {
                lengths[ref.other_id] = new_len;
                node_queue.push({ref.other_id, new_len});
            }
        });
    }
    // selecting subset of downward edges for linear sweep
    std::vector<CHEdge> down_edges_subset;
    down_edges_subset.reserve(dem_points.size());
    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    for (int i = 0; i < down_edges.size(); i++) {
        auto edge = down_edges[i];
        if (!graph_subset[edge.from]) {
            continue;
        }
        down_edges_subset.push_back(edge);
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
            explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
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
            });
        }
        // computes both down-sweep and demand-sum
        float demand_sum = 0.0;
        int length = down_edges_subset.size();
        for (int i = 0; i < length; i++) {
            auto edge = down_edges_subset[i];
            int curr_len = dist[edge.from];
            if (curr_len > max_range) {
                continue;
            }
            int dem_weight = is_demand[edge.from];
            if (dem_weight != -1) {
                float distance_decay = 1 - curr_len / (float)max_range;
                demand_sum += dem_weight * distance_decay;
            }
            int new_len = curr_len + edge.weight;
            if (dist[edge.to] > new_len) {
                dist[edge.to] = new_len;
            }
        }
        // compute R-value for facility
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            int d_dist = dist[d_node];
            if (d_dist > max_range) {
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