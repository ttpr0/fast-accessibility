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
#include "./state.h"

// PHAST
class PHAST
{
private:
    bool is_build;
    ICHGraph* graph;

public:
    PHAST(ICHGraph* graph) : graph(graph) { this->is_build = false; }

    void addMaxRange(int range) {}
    void addTarget(int id) {}

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->is_build = true;
    }

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph->nodeCount()); }
    void compute(int s_id, NodeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        calcPHAST(this->graph, s_id, state.flags);
    }
};

// RangePHAST
class RangePHAST
{
private:
    bool is_build;
    ICHGraph* graph;
    int max_range;

public:
    RangePHAST(ICHGraph* graph) : graph(graph)
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
    void addTarget(int id) {}

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->is_build = true;
    }

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph->nodeCount()); }
    void compute(int s_id, NodeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        calcRangePHAST(this->graph, s_id, state.flags, this->max_range);
    }
};

// RangePHAST+GS
class RangePHASTGS
{
private:
    bool is_build;
    CHGraph2* graph;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;

public:
    RangePHASTGS(CHGraph2* graph) : graph(graph), active_tiles(graph->tileCount() + 2, false), found_tiles(graph->tileCount() + 2, false)
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
    void addTarget(int id)
    {
        if (this->is_build) {
            return;
        }
        short tile = this->graph->getNodeTile(id);
        this->active_tiles[tile] = true;
    }

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->is_build = true;
    }

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph->nodeCount()); }
    void compute(int s_id, NodeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        for (int i = 0; i < found_tiles.size(); i++) {
            found_tiles[i] = false;
        }
        calcGSPHAST(this->graph, s_id, state.flags, this->max_range, this->active_tiles, this->found_tiles);
    }
};
