#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../../graph/ch_graph.h"
#include "../../graph/graph.h"
#include "./algorithm/range_gs_rphast.h"
#include "./algorithm/range_phast.h"
#include "./algorithm/range_rphast.h"
#include "./algorithm/rphast.h"
#include "./algorithm/rphast_preprocess.h"
#include "./state.h"

// RPHAST
template <typename TGraph = ICHGraph>
    requires any_phast_graph<TGraph>
class RPHAST
{
private:
    bool is_build;
    TGraph& graph;
    std::vector<Shortcut> down_edges_subset;
    int max_range;
    std::queue<int> node_queue;

public:
    RPHAST(TGraph& graph) : graph(graph)
    {
        this->max_range = 10000000;
        this->is_build = false;
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
            this->node_queue.push(target[i].node);
        }
    }

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->down_edges_subset = preprocessRPHAST(this->graph, std::move(this->node_queue));
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
        calcRPHAST(this->graph, start, flags, this->down_edges_subset);
    }
};

// RangeRPHAST
template <typename TGraph = ICHGraph>
    requires any_phast_graph<TGraph>
class RangeRPHAST
{
private:
    bool is_build;
    TGraph& graph;
    std::vector<Shortcut> down_edges_subset;
    int max_range;
    std::queue<int> node_queue;

public:
    RangeRPHAST(TGraph& graph) : graph(graph)
    {
        this->max_range = 10000000;
        this->is_build = false;
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
            this->node_queue.push(target[i].node);
        }
    }

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->down_edges_subset = preprocessRPHAST(this->graph, std::move(this->node_queue));
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
        calcRangeRPHAST(this->graph, start, flags, this->max_range, this->down_edges_subset);
    }
};

// RangeRPHAST (RTS)
template <typename TGraph = ICHGraph>
    requires any_phast_graph<TGraph>
class RangeRPHAST2
{
private:
    bool is_build;
    TGraph& graph;
    std::vector<Shortcut> down_edges_subset;
    int max_range;
    std::priority_queue<pq_item> node_queue;

public:
    RangeRPHAST2(TGraph& graph) : graph(graph)
    {
        this->max_range = 10000000;
        this->is_build = false;
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
            this->node_queue.push({target[i].node, target[i].dist});
        }
    }

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->down_edges_subset = preprocessRangeRPHAST(this->graph, std::move(this->node_queue), this->max_range);
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
        calcRangeRPHAST(this->graph, start, flags, this->max_range, this->down_edges_subset);
    }
};

// RangeRPHAST+GS
class RangeRPHASTGS
{
public:
    using TGraph = CHGraph2;

private:
    bool is_build;
    TGraph& graph;
    std::vector<Shortcut> down_edges_subset;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;
    std::queue<int> node_queue;

public:
    RangeRPHASTGS(TGraph& graph) : graph(graph), active_tiles(graph.tileCount() + 2, false), found_tiles(active_tiles.size(), false)
    {
        this->max_range = 100000000;
        this->is_build = false;
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
            int id = target[i].node;
            this->node_queue.push(id);
            short tile = this->graph.getNodeTile(id);
            this->active_tiles[tile] = true;
        }
    }

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->down_edges_subset = preprocessGSRPHAST(this->graph, std::move(this->node_queue));
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
        calcGSRPHAST(this->graph, start, flags, this->max_range, this->down_edges_subset, this->active_tiles, this->found_tiles);
    }
};

// RangeRPHAST+GS (RTS)
class RangeRPHASTGS2
{
public:
    using TGraph = CHGraph2;

private:
    bool is_build;
    TGraph& graph;
    std::vector<Shortcut> down_edges_subset;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;
    std::priority_queue<pq_item> node_queue;

public:
    RangeRPHASTGS2(TGraph& graph) : graph(graph), active_tiles(graph.tileCount() + 2, false), found_tiles(active_tiles.size(), false)
    {
        this->max_range = 10000000;
        this->is_build = false;
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
            int id = target[i].node;
            int dist = target[i].dist;
            this->node_queue.push({id, dist});
            short tile = this->graph.getNodeTile(id);
            this->active_tiles[tile] = true;
        }
    }

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->down_edges_subset = preprocessRangeGSRPHAST(this->graph, std::move(this->node_queue), this->max_range);
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
        calcGSRPHAST(this->graph, start, flags, this->max_range, this->down_edges_subset, this->active_tiles, this->found_tiles);
    }
};
