#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/util.h"
#include "../graph/graph.h"
#include "../util.h"
#include "../util/matrix.h"
#include "./distance_decay/decay.h"

template <class S>
Vector<float> calcReachability(S& alg, VectorView<int> dem_nodes, VectorView<int> sup_nodes, VectorView<int> sup_weights, IDistanceDecay& decay)
{
    int max_dist = decay.get_max_distance();

    if (!alg.isBuild()) {
        // prepare solver
        alg.addMaxRange(max_dist);
        for (int i = 0; i < dem_nodes.rows(); i++) {
            auto id = dem_nodes(i);
            if (id >= 0) {
                alg.addTarget(id);
            }
        }

        // build alg
        alg.build();
    }

    // create array containing accessibility results
    std::vector<std::tuple<int, int>> closest(dem_nodes.rows());
    for (int i = 0; i < closest.size(); i++) {
        closest[i] = std::make_tuple(10000000, -1);
    }

    auto state = alg.makeComputeState();
    std::mutex m;
#pragma omp parallel for firstprivate(state)
    for (int i = 0; i < sup_nodes.rows(); i++) {
        // get supply information
        int s_id = sup_nodes(i);
        if (s_id < 0) {
            continue;
        }

        // compute distances
        alg.compute(s_id, state);

        // update closest supplies
        m.lock();
        for (int i = 0; i < dem_nodes.rows(); i++) {
            int d_node = dem_nodes(i);
            if (d_node == -1) {
                continue;
            }
            int d_dist = state.getDistance(d_node);
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
    Vector<float> access(dem_nodes.rows());
    for (int i = 0; i < access.rows(); i++) {
        auto [c_dist, c_id] = closest[i];
        if (c_id == -1) {
            access(i) = -9999;
        } else {
            float distance_decay = decay.get_distance_weight(c_dist);
            access(i) = sup_weights(c_id) * distance_decay;
        }
    }
    return access;
}
