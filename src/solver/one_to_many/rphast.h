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

// RPHAST
class RPHAST
{
public:
    typedef ICHGraph Graph;
    class Builder
    {
    private:
        Graph* graph;
        std::queue<int> node_queue;
        int max_range;

    public:
        Builder(Graph* graph) : graph(graph) {}
        void addMaxRange(int range) { this->max_range = range; }
        void addTarget(int id) { this->node_queue.push(id); }
        RPHAST build()
        {
            auto down_edges_subset = preprocessRPHAST(this->graph, std::move(this->node_queue));
            return RPHAST(this->graph, std::move(down_edges_subset), this->max_range);
        }
    };

private:
    Graph* graph;
    std::vector<CHEdge> down_edges_subset;
    int max_range;
    RPHAST(Graph* graph, std::vector<CHEdge> down_edges_subset, int max_range) : graph(graph), down_edges_subset(down_edges_subset), max_range(max_range) {}

public:
    void compute(int s_id, Flags<DistFlag>& flags) { calcRPHAST(this->graph, s_id, flags, this->down_edges_subset); }
};

// RangeRPHAST
class RangeRPHAST
{
public:
    typedef ICHGraph Graph;
    class Builder
    {
    private:
        Graph* graph;
        std::queue<int> node_queue;
        int max_range;

    public:
        Builder(Graph* graph) : graph(graph) {}
        void addMaxRange(int range) { this->max_range = range; }
        void addTarget(int id) { this->node_queue.push(id); }
        RangeRPHAST build()
        {
            auto down_edges_subset = preprocessRPHAST(this->graph, std::move(this->node_queue));
            return RangeRPHAST(this->graph, std::move(down_edges_subset), this->max_range);
        }
    };

private:
    Graph* graph;
    std::vector<CHEdge> down_edges_subset;
    int max_range;
    RangeRPHAST(Graph* graph, std::vector<CHEdge> down_edges_subset, int max_range) : graph(graph), down_edges_subset(down_edges_subset), max_range(max_range) {}

public:
    void compute(int s_id, Flags<DistFlag>& flags) { calcRangeRPHAST(this->graph, s_id, flags, this->max_range, this->down_edges_subset); }
};

// RangeRPHAST (RTS)
class RangeRPHAST2
{
public:
    typedef ICHGraph Graph;
    class Builder
    {
    private:
        Graph* graph;
        std::priority_queue<pq_item> node_queue;
        int max_range;

    public:
        Builder(Graph* graph) : graph(graph) {}
        void addMaxRange(int range) { this->max_range = range; }
        void addTarget(int id) { this->node_queue.push({id, 0}); }
        RangeRPHAST2 build()
        {
            auto down_edges_subset = preprocessRangeRPHAST(this->graph, std::move(this->node_queue), this->max_range);
            return RangeRPHAST2(this->graph, std::move(down_edges_subset), this->max_range);
        }
    };

private:
    Graph* graph;
    std::vector<CHEdge> down_edges_subset;
    int max_range;

    RangeRPHAST2(Graph* graph, std::vector<CHEdge> down_edges_subset, int max_range) : graph(graph), down_edges_subset(down_edges_subset), max_range(max_range) {}

public:
    void compute(int s_id, Flags<DistFlag>& flags) { calcRangeRPHAST(this->graph, s_id, flags, this->max_range, this->down_edges_subset); }
};

// RangeRPHAST+GS
class RangeRPHASTGS
{
public:
    typedef CHGraph2 Graph;
    class Builder
    {
    private:
        Graph* graph;
        std::queue<int> node_queue;
        std::vector<bool> active_tiles;
        int max_range;

    public:
        Builder(Graph* graph) : graph(graph), active_tiles(graph->tileCount() + 2) {}
        void addMaxRange(int range) { this->max_range = range; }
        void addTarget(int id)
        {
            this->node_queue.push(id);
            short tile = this->graph->getNodeTile(id);
            this->active_tiles[tile] = true;
        }
        RangeRPHASTGS build()
        {
            auto down_edges_subset = preprocessGSRPHAST(this->graph, std::move(this->node_queue));
            return RangeRPHASTGS(this->graph, std::move(down_edges_subset), this->max_range, std::move(this->active_tiles));
        }
    };

private:
    Graph* graph;
    std::vector<CHEdge4> down_edges_subset;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;
    RangeRPHASTGS(Graph* graph, std::vector<CHEdge4> down_edges_subset, int max_range, std::vector<bool> active_tiles)
        : graph(graph), down_edges_subset(down_edges_subset), max_range(max_range), active_tiles(active_tiles), found_tiles(active_tiles.size())
    {}

public:
    void compute(int s_id, Flags<DistFlag>& flags)
    {
        for (int i = 0; i < found_tiles.size(); i++) {
            found_tiles[i] = false;
        }
        calcGSRPHAST(this->graph, s_id, flags, this->max_range, this->down_edges_subset, this->active_tiles, this->found_tiles);
    }
};

// RangeRPHAST+GS (RTS)
class RangeRPHASTGS2
{
public:
    typedef CHGraph2 Graph;
    class Builder
    {
    private:
        Graph* graph;
        std::priority_queue<pq_item> node_queue;
        std::vector<bool> active_tiles;
        int max_range;

    public:
        Builder(Graph* graph) : graph(graph), active_tiles(graph->tileCount() + 2) {}
        void addMaxRange(int range) { this->max_range = range; }
        void addTarget(int id)
        {
            this->node_queue.push({id, 0});
            short tile = this->graph->getNodeTile(id);
            this->active_tiles[tile] = true;
        }
        RangeRPHASTGS2 build()
        {
            auto down_edges_subset = preprocessRangeGSRPHAST(this->graph, std::move(this->node_queue), this->max_range);
            return RangeRPHASTGS2(this->graph, std::move(down_edges_subset), this->max_range, std::move(this->active_tiles));
        }
    };

private:
    Graph* graph;
    std::vector<CHEdge4> down_edges_subset;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;
    RangeRPHASTGS2(Graph* graph, std::vector<CHEdge4> down_edges_subset, int max_range, std::vector<bool> active_tiles)
        : graph(graph), down_edges_subset(down_edges_subset), max_range(max_range), active_tiles(active_tiles), found_tiles(active_tiles.size())
    {}

public:
    void compute(int s_id, Flags<DistFlag>& flags)
    {
        for (int i = 0; i < found_tiles.size(); i++) {
            found_tiles[i] = false;
        }
        calcGSRPHAST(this->graph, s_id, flags, this->max_range, this->down_edges_subset, this->active_tiles, this->found_tiles);
    }
};
