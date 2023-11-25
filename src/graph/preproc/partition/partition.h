#pragma once

#include <vector>

#include "../../base_graph.h"
#include "../../comps/graph_base.h"
#include "../../comps/partition.h"
#include "../../comps/weighting.h"
#include "./inertial_flow.h"

std::shared_ptr<Partition> calc_partition(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weight, int count)
{
    Graph graph = build_base_graph(base, weight);

    auto tiles = InertialFlow(graph, count);

    return std::make_shared<Partition>(std::move(tiles));
}
