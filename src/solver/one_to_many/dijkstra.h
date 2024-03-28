#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../../graph/graph.h"
#include "./algorithm/range_dijkstra.h"
#include "./state.h"

// RangeDijkstra
template <typename TGraph = IGraph>
    requires any_graph<TGraph>
class RangeDijkstra
{
private:
    bool is_build;
    TGraph& graph;
    int max_range;

public:
    RangeDijkstra(TGraph& graph) : graph(graph)
    {
        this->is_build = false;
        this->max_range = 10000000;
    }
    void addMaxRange(int range)
    {
        if (this->is_build) {
            return;
        }
        this->max_range = range;
    }
    void addTarget(DSnap target) {}

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->is_build = true;
    }

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph.nodeCount()); }
    void compute(DSnap start, NodeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        auto& flags = state.flags;
        flags.soft_reset();
        calcRangeDijkstra(this->graph, start, flags, this->max_range);
    }
};

// RangeDijkstra with turn-costs
template <typename TGraph = IGraph>
    requires any_tc_graph<TGraph>
class RangeDijkstraTC
{
private:
    bool is_build;
    TGraph& graph;
    int max_range;

public:
    RangeDijkstraTC(TGraph& graph) : graph(graph)
    {
        this->is_build = false;
        this->max_range = 10000000;
    }
    void addMaxRange(int range)
    {
        if (this->is_build) {
            return;
        }
        this->max_range = range;
    }
    void addTarget(DSnap target) {}

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->is_build = true;
    }

    EdgeBasedState makeComputeState() { return EdgeBasedState(this->graph.nodeCount(), this->graph.edgeCount()); }
    void compute(DSnap start, EdgeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        auto& node_flags = state.node_flags;
        auto& edge_flags = state.edge_flags;
        node_flags.soft_reset();
        edge_flags.soft_reset();
        calcRangeDijkstraTC(this->graph, start, node_flags, edge_flags, this->max_range);
    }
};
