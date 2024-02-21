#pragma once

#include "../../algorithm/util.h"

class NodeBasedState
{
public:
    Flags<DistFlag> flags;

    NodeBasedState(int node_count) : flags(node_count, {10000000, false}) {}

    int getDistance(int id)
    {
        auto& node = this->flags[id];
        return node.dist;
    }
};

class EdgeBasedState
{
public:
    Flags<DistFlag> node_flags;
    Flags<DistFlag> edge_flags;

    EdgeBasedState(int node_count, int edge_count) : node_flags(node_count, {10000000, false}), edge_flags(edge_count, {10000000, false}) {}

    int getDistance(int id)
    {
        auto& node = this->node_flags[id];
        return node.dist;
    }
};
