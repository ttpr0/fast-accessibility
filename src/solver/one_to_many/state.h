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