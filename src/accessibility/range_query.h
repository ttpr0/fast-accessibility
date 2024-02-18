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
Vector<int> calcRangeQuery(S& alg, int start_node, VectorView<int> target_nodes, int max_range)
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

    // compute flags
    auto state = alg.makeComputeState();
    alg.compute(start_node, state);

    // create array containing accessibility results
    Vector<int> ranges(target_nodes.rows());
    for (int i = 0; i < ranges.rows(); i++) {
        int target_node = target_nodes(i);
        if (target_node == -1) {
            ranges(i) = -9999;
            continue;
        }
        auto target_dist = state.getDistance(target_node);
        if (target_dist > max_range) {
            ranges(i) = -9999;
        } else {
            ranges(i) = target_dist;
        }
    }

    return ranges;
}
