#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../../algorithm/range_dijkstra.h"
#include "../../algorithm/util.h"
#include "../../graph/base_graph.h"
#include "../../graph/graph.h"
#include "../../util.h"
#include "./state.h"

// RangeDijkstra
class RangeDijkstra
{
private:
    bool is_build;
    IGraph* graph;
    int max_range;

public:
    RangeDijkstra(IGraph* graph) : graph(graph)
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

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph->nodeCount()); }
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
class RangeDijkstraTC
{
private:
    bool is_build;
    IGraph* graph;
    int max_range;

public:
    RangeDijkstraTC(IGraph* graph) : graph(graph)
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

    EdgeBasedState makeComputeState() { return EdgeBasedState(this->graph->nodeCount(), this->graph->edgeCount()); }
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
