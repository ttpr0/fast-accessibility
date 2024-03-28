#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../util/snap.h"
#include "../graph/graph.h"
#include "../util/matrix.h"

template <class S>
Vector<int> calcRangeQuery(S& alg, DSnap start_node, std::vector<DSnap>& target_nodes, int max_range)
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

    // compute flags
    auto state = alg.makeComputeState();
    alg.compute(start_node, state);

    // create array containing accessibility results
    Vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        auto t_snap = target_nodes[i];
        if (t_snap.len() == 0) {
            ranges(i) = -9999;
            continue;
        }
        auto target_dist = state.getDistance(t_snap);
        if (target_dist > max_range) {
            ranges(i) = -9999;
        } else {
            ranges(i) = target_dist;
        }
    }

    return ranges;
}
