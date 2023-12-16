#pragma once

#include <unordered_map>
#include <vector>

#include "../structs/structs.h"

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
