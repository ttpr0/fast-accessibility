#pragma once

#include <unordered_map>
#include <vector>

#include "./ch_data.h"
#include "./graph_base.h"
#include "./partition.h"
#include "../structs/structs.h"
#include "./weighting.h"

class _CHIndex
{
public:
    std::vector<CHEdge> fwd_down_edges;
    std::vector<CHEdge> bwd_down_edges;
};

class _CHIndex2
{
public:
    std::vector<CHEdge4> fwd_down_edges;
    std::vector<CHEdge4> bwd_down_edges;
};

std::shared_ptr<_CHIndex> build_ch_index(GraphBase& base, Weighting& weight, CHData& ch);
std::shared_ptr<_CHIndex2> build_ch_index_2(GraphBase& base, Weighting& weight, CHData& ch, Partition& partition);
