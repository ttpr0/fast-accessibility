#pragma once

#include "../../algorithm/util.h"

class NodeBasedState
{
public:
    Flags<DistFlag> flags;

    NodeBasedState(int node_count) : flags(node_count, {10000000, false}) {}

    int getDistance(DSnap target)
    {
        int min_dist = 10000000;
        for (int i = 0; i < target.len(); i++) {
            int id = target[i].node;
            int dist = target[i].dist;
            auto& node = this->flags[id];
            if (node.dist + dist < min_dist) {
                min_dist = node.dist + dist;
            }
        }
        return min_dist;
    }
};

class EdgeBasedState
{
public:
    Flags<DistFlag> node_flags;
    Flags<DistFlag> edge_flags;

    EdgeBasedState(int node_count, int edge_count) : node_flags(node_count, {10000000, false}), edge_flags(edge_count, {10000000, false}) {}

    int getDistance(DSnap target)
    {
        int min_dist = 10000000;
        for (int i = 0; i < target.len(); i++) {
            int id = target[i].node;
            int dist = target[i].dist;
            auto& node = this->node_flags[id];
            if (node.dist + dist < min_dist) {
                min_dist = node.dist + dist;
            }
        }
        return min_dist;
    }
};

class TransitState
{
public:
    Flags<DistFlag> node_flags;
    Flags<DistFlag> edge_flags;
    Flags<TransitFlag> stop_flags;

    TransitState(int node_count, int edge_count, int stop_count) : node_flags(node_count, {10000000, false}), edge_flags(edge_count, {10000000, false}), stop_flags(stop_count, {})
    {}

    int getDistance(DSnap target)
    {
        int min_dist = 10000000;
        for (int i = 0; i < target.len(); i++) {
            int id = target[i].node;
            int dist = target[i].dist;
            auto& node = this->node_flags[id];
            if (node.dist + dist < min_dist) {
                min_dist = node.dist + dist;
            }
        }
        return min_dist;
    }
};
