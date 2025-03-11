#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "../util.h"
#include "../util/matrix.h"
#include "../util/snap.h"
#include "./distance_decay/decay.h"

template <class S>
Vector<float> _calc2SFCA(S& alg, std::vector<DSnap>& dem_nodes, VectorView<int> dem_weights, std::vector<DSnap>& sup_nodes, VectorView<int> sup_weights, IDistanceDecay& decay)
{
    int max_dist = decay.get_max_distance();

    if (!alg.isBuild()) {
        // preprare solver
        alg.addMaxRange(max_dist);
        for (int i = 0; i < dem_nodes.size(); i++) {
            auto snap = dem_nodes[i];
            if (snap.len() > 0) {
                alg.addTarget(snap);
            }
        }

        // build solver
        alg.build();
    }

    // create array containing accessibility results
    Vector<float> access(dem_nodes.size());
    access.setZero();

    auto state = alg.makeComputeState();
    std::mutex m;
#pragma omp parallel for firstprivate(state)
    for (int i = 0; i < sup_nodes.size(); i++) {
        // get supply information
        auto s_snap = sup_nodes[i];
        if (s_snap.len() == 0) {
            continue;
        }
        int s_weight = sup_weights(i);

        // compute distances
        alg.compute(s_snap, state);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            auto d_snap = dem_nodes[i];
            if (d_snap.len() == 0) {
                continue;
            }
            int d_dist = state.getDistance(d_snap);
            if (d_dist >= max_dist) {
                continue;
            }
            float distance_decay = decay.get_distance_weight(d_dist);
            demand_sum += dem_weights(i) * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            auto d_snap = dem_nodes[i];
            if (d_snap.len() == 0) {
                continue;
            }
            int d_dist = state.getDistance(d_snap);
            if (d_dist >= max_dist) {
                continue;
            }
            float distance_decay = decay.get_distance_weight(d_dist);
            access(i) += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}

template <class S>
Vector<float> calc2SFCA(S& alg, std::vector<DSnap>& dem_nodes, VectorView<int> dem_weights, std::vector<DSnap>& sup_nodes, VectorView<int> sup_weights, IDistanceDecay& decay)
{
    int max_dist = decay.get_max_distance();

    if (!alg.isBuild()) {
        // preprare solver
        alg.addMaxRange(max_dist);
        for (int i = 0; i < dem_nodes.size(); i++) {
            auto snap = dem_nodes[i];
            if (snap.len() > 0) {
                alg.addTarget(snap);
            }
        }

        // build solver
        alg.build();
    }

    // create array containing accessibility results
    Vector<float> access(dem_nodes.size());
    access.setZero();

    auto state = alg.makeComputeState();
    std::mutex m;
#pragma omp parallel firstprivate(state)
    {
        // create array containing accessibility results
        std::vector<float> partial_access(dem_nodes.size(), 0);
#pragma omp for
        for (int i = 0; i < sup_nodes.size(); i++) {
            // get supply information
            auto s_snap = sup_nodes[i];
            if (s_snap.len() == 0) {
                continue;
            }
            int s_weight = sup_weights(i);

            // compute distances
            alg.compute(s_snap, state);

            // compute R-value for facility
            float demand_sum = 0.0;
            for (int j = 0; j < dem_nodes.size(); j++) {
                auto d_snap = dem_nodes[j];
                if (d_snap.len() == 0) {
                    continue;
                }
                int d_dist = state.getDistance(d_snap);
                if (d_dist > max_dist) {
                    continue;
                }
                float distance_decay = decay.get_distance_weight(d_dist);
                demand_sum += dem_weights(j) * distance_decay;
            }
            float R = s_weight / demand_sum;
            // add new access to reachable demand points
            for (int j = 0; j < dem_nodes.size(); j++) {
                auto d_snap = dem_nodes[j];
                if (d_snap.len() == 0) {
                    continue;
                }
                int d_dist = state.getDistance(d_snap);
                if (d_dist > max_dist) {
                    continue;
                }
                float distance_decay = decay.get_distance_weight(d_dist);
                partial_access[j] += R * distance_decay;
            }
        }

        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            access(i) += partial_access[i];
        }
        m.unlock();
    }
    return access;
}
