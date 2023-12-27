#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../../algorithm/range_gs_rphast.h"
#include "../../algorithm/range_phast.h"
#include "../../algorithm/range_rphast.h"
#include "../../algorithm/rphast.h"
#include "../../algorithm/rphast_preprocess.h"
#include "../../algorithm/util.h"
#include "../../graph/ch_graph.h"
#include "../../graph/graph.h"
#include "../../util.h"
#include "./state.h"

// RPHAST
class RPHAST
{
private:
    bool is_build;
    ICHGraph* graph;
    std::vector<CHEdge> down_edges_subset;
    int max_range;
    std::queue<int> node_queue;

public:
    RPHAST(ICHGraph* graph) : graph(graph)
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
    void addTarget(int id)
    {
        if (this->is_build) {
            return;
        }
        this->node_queue.push(id);
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

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph->nodeCount()); }
    void compute(int s_id, NodeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        calcRPHAST(this->graph, s_id, state.flags, this->down_edges_subset);
    }
};

// RangeRPHAST
class RangeRPHAST
{
private:
    bool is_build;
    ICHGraph* graph;
    std::vector<CHEdge> down_edges_subset;
    int max_range;
    std::queue<int> node_queue;

public:
    RangeRPHAST(ICHGraph* graph) : graph(graph)
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
    void addTarget(int id)
    {
        if (this->is_build) {
            return;
        }
        this->node_queue.push(id);
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

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph->nodeCount()); }
    void compute(int s_id, NodeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        calcRangeRPHAST(this->graph, s_id, state.flags, this->max_range, this->down_edges_subset);
    }
};

// RangeRPHAST (RTS)
class RangeRPHAST2
{
private:
    bool is_build;
    ICHGraph* graph;
    std::vector<CHEdge> down_edges_subset;
    int max_range;
    std::priority_queue<pq_item> node_queue;

public:
    RangeRPHAST2(ICHGraph* graph) : graph(graph)
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
    void addTarget(int id)
    {
        if (this->is_build) {
            return;
        }
        this->node_queue.push({id, 0});
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

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph->nodeCount()); }
    void compute(int s_id, NodeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        calcRangeRPHAST(this->graph, s_id, state.flags, this->max_range, this->down_edges_subset);
    }
};

// RangeRPHAST+GS
class RangeRPHASTGS
{
private:
    bool is_build;
    CHGraph2* graph;
    std::vector<CHEdge4> down_edges_subset;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;
    std::queue<int> node_queue;

public:
    RangeRPHASTGS(CHGraph2* graph) : graph(graph), active_tiles(graph->tileCount() + 2, false), found_tiles(active_tiles.size(), false)
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
    void addTarget(int id)
    {
        if (this->is_build) {
            return;
        }
        this->node_queue.push(id);
        short tile = this->graph->getNodeTile(id);
        this->active_tiles[tile] = true;
    }

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        auto down_edges_subset = preprocessGSRPHAST(this->graph, std::move(this->node_queue));
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
        calcGSRPHAST(this->graph, s_id, state.flags, this->max_range, this->down_edges_subset, this->active_tiles, this->found_tiles);
    }
};

// RangeRPHAST+GS (RTS)
class RangeRPHASTGS2
{
private:
    bool is_build;
    CHGraph2* graph;
    std::vector<CHEdge4> down_edges_subset;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;
    std::priority_queue<pq_item> node_queue;

public:
    RangeRPHASTGS2(CHGraph2* graph) : graph(graph), active_tiles(graph->tileCount() + 2, false), found_tiles(active_tiles.size(), false)
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
    void addTarget(int id)
    {
        if (this->is_build) {
            return;
        }
        this->node_queue.push({id, 0});
        short tile = this->graph->getNodeTile(id);
        this->active_tiles[tile] = true;
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

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph->nodeCount()); }
    void compute(int s_id, NodeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        for (int i = 0; i < found_tiles.size(); i++) {
            found_tiles[i] = false;
        }
        calcGSRPHAST(this->graph, s_id, state.flags, this->max_range, this->down_edges_subset, this->active_tiles, this->found_tiles);
    }
};
