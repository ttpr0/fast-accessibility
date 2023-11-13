#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../../algorithm/phast.h"
#include "../../algorithm/range_gs_phast.h"
#include "../../algorithm/range_phast.h"
#include "../../algorithm/util.h"
#include "../../graph/ch_graph.h"
#include "../../graph/graph.h"
#include "../../util.h"

// PHAST
class PHAST
{
public:
    typedef CHGraph Graph;
    class Builder
    {
    private:
        Graph* graph;

    public:
        Builder(Graph* graph) : graph(graph) {}
        void addMaxRange(int range) {}
        void addTarget(int id) {}
        PHAST build() { return PHAST(this->graph); }
    };

private:
    Graph* graph;
    PHAST(Graph* graph) : graph(graph) {}

public:
    void compute(int s_id, DistFlagArray& flags) { calcPHAST(this->graph, s_id, flags); }
};

// RangePHAST
class RangePHAST
{
public:
    typedef CHGraph Graph;
    class Builder
    {
    private:
        Graph* graph;
        int max_range;

    public:
        Builder(Graph* graph) : graph(graph) {}
        void addMaxRange(int range) { this->max_range = range; }
        void addTarget(int id) {}
        RangePHAST build() { return RangePHAST(this->graph, this->max_range); }
    };

private:
    Graph* graph;
    int max_range;
    RangePHAST(Graph* graph, int max_range) : graph(graph), max_range(max_range) {}

public:
    void compute(int s_id, DistFlagArray& flags) { calcRangePHAST(this->graph, s_id, flags, this->max_range); }
};

// RangePHAST+GS
class RangePHASTGS
{
public:
    typedef CHGraph2 Graph;
    class Builder
    {
    private:
        Graph* graph;
        std::vector<bool> active_tiles;
        int max_range;

    public:
        Builder(Graph* graph) : graph(graph), active_tiles(graph->tileCount() + 2) {}
        void addMaxRange(int range) { this->max_range = range; }
        void addTarget(int id)
        {
            short tile = this->graph->getNodeTile(id);
            this->active_tiles[tile] = true;
        }
        RangePHASTGS build() { return RangePHASTGS(this->graph, this->max_range, std::move(this->active_tiles)); }
    };

private:
    Graph* graph;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;
    RangePHASTGS(Graph* graph, int max_range, std::vector<bool> active_tiles)
        : graph(graph), max_range(max_range), active_tiles(active_tiles), found_tiles(active_tiles.size())
    {}

public:
    void compute(int s_id, DistFlagArray& flags)
    {
        for (int i = 0; i < found_tiles.size(); i++) {
            found_tiles[i] = false;
        }
        calcGSPHAST(this->graph, s_id, flags, this->max_range, this->active_tiles, this->found_tiles);
    }
};