#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/range_dijkstra.h"
#include "../algorithm/util.h"
#include "../graph/graph.h"

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

    auto flags = DistFlagArray(g->nodeCount());
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
            m.lock();
            access[i] += R * distance_decay;
            m.unlock();
        }
    }
    return access;
}
