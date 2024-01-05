#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/range_gs_rphast.h"
#include "../algorithm/range_phast.h"
#include "../algorithm/range_rphast.h"
#include "../algorithm/rphast.h"
#include "../algorithm/rphast_preprocess.h"
#include "../algorithm/util.h"
#include "../graph/graph.h"
#include "../util.h"
#include "./distance_decay/decay.h"

template <class S>
std::vector<float> _calc2SFCA(S& alg, std::vector<int>& dem_nodes, std::vector<int>& dem_weights, std::vector<int>& sup_nodes, std::vector<int>& sup_weights, IDistanceDecay& decay)
{
    int max_dist = decay.get_max_distance();

    if (!alg.isBuild()) {
        // preprare solver
        alg.addMaxRange(max_dist);
        for (int i = 0; i < dem_nodes.size(); i++) {
            auto id = dem_nodes[i];
            if (id >= 0) {
                alg.addTarget(id);
            }
        }

        // build solver
        alg.build();
    }

    // create array containing accessibility results
    std::vector<float> access(dem_nodes.size(), 0);

    auto state = alg.makeComputeState();
    std::mutex m;
#pragma omp parallel for firstprivate(state)
    for (int i = 0; i < sup_nodes.size(); i++) {
        // get supply information
        int s_id = sup_nodes[i];
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        alg.compute(s_id, state);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            int d_dist = state.getDistance(d_node);
            if (d_dist >= max_dist) {
                continue;
            }
            float distance_decay = decay.get_distance_weight(d_dist);
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
            int d_dist = state.getDistance(d_node);
            if (d_dist >= max_dist) {
                continue;
            }
            float distance_decay = decay.get_distance_weight(d_dist);
            access[i] += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}

template <class S>
std::vector<float> calc2SFCA(S& alg, std::vector<int>& dem_nodes, std::vector<int>& dem_weights, std::vector<int>& sup_nodes, std::vector<int>& sup_weights, IDistanceDecay& decay)
{
    int max_dist = decay.get_max_distance();

    if (!alg.isBuild()) {
        // preprare solver
        alg.addMaxRange(max_dist);
        for (int i = 0; i < dem_nodes.size(); i++) {
            auto id = dem_nodes[i];
            if (id >= 0) {
                alg.addTarget(id);
            }
        }

        // build solver
        alg.build();
    }

    // create array containing accessibility results
    std::vector<float> access(dem_nodes.size(), 0);

    auto state = alg.makeComputeState();
    std::mutex m;
#pragma omp parallel firstprivate(state)
    {
        // create array containing accessibility results
        std::vector<float> partial_access(dem_nodes.size(), 0);
#pragma omp for
        for (int i = 0; i < sup_nodes.size(); i++) {
            // get supply information
            int s_id = sup_nodes[i];
            if (s_id < 0) {
                continue;
            }
            int s_weight = sup_weights[i];

            // compute distances
            alg.compute(s_id, state);

            // compute R-value for facility
            float demand_sum = 0.0;
            for (int i = 0; i < dem_nodes.size(); i++) {
                int d_node = dem_nodes[i];
                if (d_node == -1) {
                    continue;
                }
                int d_dist = state.getDistance(d_node);
                if (d_dist >= max_dist) {
                    continue;
                }
                float distance_decay = decay.get_distance_weight(d_dist);
                demand_sum += dem_weights[i] * distance_decay;
            }
            float R = s_weight / demand_sum;
            // add new access to reachable demand points
            for (int i = 0; i < dem_nodes.size(); i++) {
                int d_node = dem_nodes[i];
                if (d_node == -1) {
                    continue;
                }
                int d_dist = state.getDistance(d_node);
                if (d_dist >= max_dist) {
                    continue;
                }
                float distance_decay = decay.get_distance_weight(d_dist);
                partial_access[i] += R * distance_decay;
            }
        }

        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            access[i] += partial_access[i];
        }
        m.unlock();
    }
    return access;
}
