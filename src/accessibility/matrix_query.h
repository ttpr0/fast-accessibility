#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/util.h"
#include "../graph/graph.h"
#include "../util/matrix.h"

template <class S>
Matrix<int> calcMatrixQuery(S& alg, VectorView<int> start_nodes, VectorView<int> target_nodes, int max_range)
{
    if (!alg.isBuild()) {
        // prepare solver
        alg.addMaxRange(max_range);
        for (int i = 0; i < target_nodes.rows(); i++) {
            auto id = target_nodes(i);
            if (id >= 0) {
                alg.addTarget(id);
            }
        }

        // build solver
        alg.build();
    }

    // create matrix containing accessibility results
    Matrix<int> ranges(start_nodes.rows(), target_nodes.rows());

    auto state = alg.makeComputeState();
#pragma omp parallel for firstprivate(state)
    for (int i = 0; i < start_nodes.rows(); i++) {
        // get start id
        int s_id = start_nodes(i);
        if (s_id < 0) {
            for (int j = 0; j < target_nodes.rows(); j++) {
                ranges(i, j) = -9999;
            }
            continue;
        }
        // compute ranges
        alg.compute(s_id, state);
        // extract ranges of targets
        for (int j = 0; j < target_nodes.rows(); j++) {
            int t_id = target_nodes(j);
            if (t_id == -1) {
                ranges(i, j) = -9999;
                continue;
            }
            auto dist = state.getDistance(t_id);
            if (dist > max_range) {
                ranges(i, j) = -9999;
            } else {
                ranges(i, j) = dist;
            }
        }
    }

    return ranges;
}
