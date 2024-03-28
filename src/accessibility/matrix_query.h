#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "../util/matrix.h"
#include "../util/snap.h"

template <class S>
Matrix<int> calcMatrixQuery(S& alg, std::vector<DSnap>& start_nodes, std::vector<DSnap>& target_nodes, int max_range)
{
    if (!alg.isBuild()) {
        // prepare solver
        alg.addMaxRange(max_range);
        for (int i = 0; i < target_nodes.size(); i++) {
            auto snap = target_nodes[i];
            if (snap.len() > 0) {
                alg.addTarget(snap);
            }
        }

        // build solver
        alg.build();
    }

    // create matrix containing accessibility results
    Matrix<int> ranges(start_nodes.size(), target_nodes.size());

    auto state = alg.makeComputeState();
#pragma omp parallel for firstprivate(state)
    for (int i = 0; i < start_nodes.size(); i++) {
        // get start id
        auto s_snap = start_nodes[i];
        if (s_snap.len() == 0) {
            for (int j = 0; j < target_nodes.size(); j++) {
                ranges(i, j) = -9999;
            }
            continue;
        }
        // compute ranges
        alg.compute(s_snap, state);
        // extract ranges of targets
        for (int j = 0; j < target_nodes.size(); j++) {
            auto t_snap = target_nodes[j];
            if (t_snap.len() == 0) {
                ranges(i, j) = -9999;
                continue;
            }
            auto dist = state.getDistance(t_snap);
            if (dist > max_range) {
                ranges(i, j) = -9999;
            } else {
                ranges(i, j) = dist;
            }
        }
    }

    return ranges;
}
