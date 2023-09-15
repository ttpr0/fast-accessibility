#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/pq_item.h"
#include "../graph/graph.h"

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

    std::vector<bool> visited(g->nodeCount());
    std::vector<int> dist(g->nodeCount());
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
            explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
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
            float distance_decay = dist[d_node] / max_range;
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
            float distance_decay = dist[d_node] / max_range;
            access[i] += R * distance_decay;
        }
    }
    return access;
}