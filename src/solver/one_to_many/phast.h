#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "./algorithm/phast.h"
#include "./algorithm/range_gs_phast.h"
#include "./algorithm/range_phast.h"
#include "../../graph/ch_graph.h"
#include "../../graph/graph.h"
#include "./state.h"

// PHAST
template <typename TGraph = ICHGraph>
    requires any_phast_graph<TGraph>
class PHAST
{
private:
    bool is_build;
    TGraph& graph;

public:
    PHAST(TGraph& graph) : graph(graph) { this->is_build = false; }

    void addMaxRange(int range) {}
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
        calcPHAST(this->graph, start, flags);
    }
};

// RangePHAST
template <typename TGraph = ICHGraph>
    requires any_phast_graph<TGraph>
class RangePHAST
{
private:
    bool is_build;
    TGraph& graph;
    int max_range;

public:
    RangePHAST(TGraph& graph) : graph(graph)
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
        calcRangePHAST(this->graph, start, flags, this->max_range);
    }
};

// RangePHAST+GS
class RangePHASTGS
{
public:
    using TGraph = CHGraph2;

private:
    bool is_build;
    TGraph& graph;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;

public:
    RangePHASTGS(TGraph& graph) : graph(graph), active_tiles(graph.tileCount() + 2, false), found_tiles(graph.tileCount() + 2, false)
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
    void addTarget(DSnap target)
    {
        if (this->is_build) {
            return;
        }
        for (int i = 0; i < target.len(); i++) {
            short tile = this->graph.getNodeTile(target[i].node);
            this->active_tiles[tile] = true;
        }
    }

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
        for (int i = 0; i < found_tiles.size(); i++) {
            found_tiles[i] = false;
        }
        auto& flags = state.flags;
        flags.soft_reset();
        calcGSPHAST(this->graph, start, flags, this->max_range, this->active_tiles, this->found_tiles);
    }
};
