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

// RangeDijkstra
class RangeDijkstra
{
public:
    typedef IGraph Graph;
    class Builder
    {
    private:
        Graph* graph;
        int max_range;

    public:
        Builder(Graph* graph) : graph(graph) {}
        void addMaxRange(int range) { this->max_range = range; }
        void addTarget(int id) {}
        RangeDijkstra build() { return RangeDijkstra(this->graph, this->max_range); }
    };

private:
    Graph* graph;
    int max_range;
    RangeDijkstra(Graph* graph, int max_range) : graph(graph), max_range(max_range) {}

public:
    void compute(int s_id, DistFlagArray& flags) { calcRangeDijkstra(this->graph, s_id, flags, this->max_range); }
};