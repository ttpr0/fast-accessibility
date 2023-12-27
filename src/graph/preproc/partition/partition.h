#pragma once

#include <vector>

#include "../../base_graph.h"
#include "../../comps/graph_base.h"
#include "../../comps/weighting.h"
#include "../../speed_ups/partition.h"
#include "./inertial_flow.h"

static std::shared_ptr<Partition> calc_partition(const GraphBase& base, const Weighting& weight, int count)
{
    auto tiles = inertial_flow(base, weight, count);

    return std::make_shared<Partition>(std::move(tiles));
}
