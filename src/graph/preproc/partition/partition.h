#pragma once

#include <vector>

#include "../../base_graph.h"
#include "../../comps/graph_base.h"
#include "../../comps/weighting.h"
#include "../../speed_ups/partition.h"
#include "./inertial_flow.h"

std::shared_ptr<Partition> calc_partition(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weight, int count)
{
    auto index = build_base_index(*base);
    Graph graph = build_base_graph(base, weight, index);

    auto tiles = InertialFlow(graph, count);

    return std::make_shared<Partition>(std::move(tiles));
}
