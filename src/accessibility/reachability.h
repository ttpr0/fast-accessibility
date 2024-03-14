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
Vector<float> calcReachability(S& alg, std::vector<DSnap>& dem_nodes, std::vector<DSnap>& sup_nodes, VectorView<int> sup_weights, IDistanceDecay& decay)
{
    int max_dist = decay.get_max_distance();

    if (!alg.isBuild()) {
        // prepare solver
        alg.addMaxRange(max_dist);
        for (int i = 0; i < dem_nodes.size(); i++) {
            auto snap = dem_nodes[i];
            if (snap.len() > 0) {
                alg.addTarget(snap);
            }
        }

        // build alg
        alg.build();
    }

    // create array containing accessibility results
    std::vector<std::tuple<int, int>> closest(dem_nodes.size());
    for (int i = 0; i < closest.size(); i++) {
        closest[i] = std::make_tuple(10000000, -1);
    }

    auto state = alg.makeComputeState();
    std::mutex m;
#pragma omp parallel for firstprivate(state)
    for (int i = 0; i < sup_nodes.size(); i++) {
        // get supply information
        auto s_snap = sup_nodes[i];
        if (s_snap.len() == 0) {
            continue;
        }

        // compute distances
        alg.compute(s_snap, state);

        // update closest supplies
        m.lock();
        for (int j = 0; j < dem_nodes.size(); j++) {
            auto d_snap = dem_nodes[j];
            if (d_snap.len() == 0) {
                continue;
            }
            int d_dist = state.getDistance(d_snap);
            if (d_dist > max_dist) {
                continue;
            }
            auto [c_dist, c_id] = closest[i];
            if (d_dist < c_dist || c_id == -1) {
                closest[j] = std::make_tuple(d_dist, i);
            }
        }
        m.unlock();
    }

    // create array containing reachability results
    Vector<float> access(dem_nodes.size());
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
