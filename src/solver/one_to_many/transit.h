#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../../graph/graph.h"
#include "../../graph/transit_graph.h"
#include "./algorithm/transit_dijkstra.h"
#include "./state.h"

// RangeDijkstra with public-transit
class TransitDijkstra
{
private:
    bool is_build;
    TransitGraph& graph;
    int max_range;
    int min_departure;
    int max_departure;

public:
    TransitDijkstra(TransitGraph& graph) : graph(graph)
    {
        this->is_build = false;
        this->max_range = 10000000;
        this->min_departure = 0;
        this->max_departure = 10000000;
    }
    void addMaxRange(int range)
    {
        if (this->is_build) {
            return;
        }
        this->max_range = range;
    }
    void addTarget(DSnap target) {}
    void setMinDeparture(int departure) { this->min_departure = departure; }
    void setMaxDeparture(int departure) { this->max_departure = departure; }

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->is_build = true;
    }

    TransitState makeComputeState() { return TransitState(this->graph.getBaseGraph().nodeCount(), this->graph.getBaseGraph().edgeCount(), this->graph.stopCount()); }
    void compute(DSnap start, TransitState& state)
    {
        if (!this->is_build) {
            return;
        }
        auto& node_flags = state.node_flags;
        auto& edge_flags = state.edge_flags;
        auto& stop_flags = state.stop_flags;
        node_flags.soft_reset();
        edge_flags.soft_reset();
        stop_flags.soft_reset();
        calcTransitDijkstra(this->graph, start, node_flags, edge_flags, stop_flags, this->max_range, this->min_departure, this->max_departure);
    }
};