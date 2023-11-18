#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/util.h"
#include "../graph/graph.h"

template <class S>
std::vector<int> calcRangeQuery(typename S::Graph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    typename S::Builder alg_builder(g);
    alg_builder.addMaxRange(max_range);
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    for (int i = 0; i < target_points.size(); i++) {
        auto loc = target_points[i];
        auto id = index.getClosestNode(loc);
        target_nodes[i] = id;
        if (id >= 0) {
            alg_builder.addTarget(id);
        }
    }

    // build alg
    auto alg = alg_builder.build();

    // compute flags
    auto flags = Flags<DistFlag>(g->nodeCount(), {10000000, false});
    alg.compute(start_node, flags);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}
