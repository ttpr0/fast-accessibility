#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/util.h"
#include "../graph/graph.h"
#include "../util.h"
#include "./distance_decay/decay.h"

template <class S>
std::vector<float> calcReachability(typename S::Graph* g, std::vector<Coord>& dem_points, std::vector<Coord>& sup_points, std::vector<int>& sup_weights, IDistanceDecay& decay)
{
    typename S::Builder alg_builder(g);
    int max_dist = decay.get_max_distance();
    alg_builder.addMaxRange(max_dist);
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            alg_builder.addTarget(id);
        }
    }

    // build alg
    auto alg = alg_builder.build();

    // create array containing accessibility results
    std::vector<std::tuple<int, int>> closest(dem_points.size());
    for (int i = 0; i < closest.size(); i++) {
        closest[i] = std::make_tuple(10000000, -1);
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

        // compute distances
        flags.soft_reset();
        alg.compute(s_id, flags);

        // update closest supplies
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_dist) {
                continue;
            }
            auto [c_dist, c_id] = closest[i];
            if (d_dist < c_dist || c_id == -1) {
                closest[i] = std::make_tuple(d_dist, s_id);
            }
        }
        m.unlock();
    }

    // create array containing reachability results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        auto [c_dist, c_id] = closest[i];
        if (c_id == -1) {
            access[i] = -9999;
        } else {
            float distance_decay = decay.get_distance_weight(c_dist);
            access[i] = sup_weights[c_id] * distance_decay;
        }
    }
    return access;
}
