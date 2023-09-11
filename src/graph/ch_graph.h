#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "./base_graph.h"
#include "./ch_storage.h"
#include "./graph.h"
#include "./graph_storage.h"
#include "./topology_storage.h"

//*******************************************
// base-graph
//******************************************

class CHGraph : public ICHGraph
{
public:
    GraphStore store;
    TopologyStore topology;
    std::vector<int> edge_weights;

    // additional ch components
    CHStore ch_store;
    TopologyStore ch_topology;
    // stores all fowwards-down edges
    std::vector<CHEdge> fwd_down_edges;
    // stores all backwards-down edges
    std::vector<CHEdge> bwd_down_edges;

    CHGraph(GraphStore store, TopologyStore topology, std::vector<int> weights,
            CHStore ch_store, TopologyStore ch_topology);

    std::unique_ptr<IGraphExplorer> getGraphExplorer();
    std::unique_ptr<IGraphIndex> getIndex();
    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);
    Coord getNodeGeom(int node);

    short getNodeLevel(int node);
    int shortcutCount();
    CHShortcut getShortcut(int shortcut);
    std::tuple<CHEdge*, int> getDownEdges(Direction dir);
};

//*******************************************
// ch-graph explorer
//******************************************

class CHGraphExplorer : public IGraphExplorer
{
public:
    CHGraph* graph;
    TopologyAccessor accessor;
    TopologyAccessor sh_accessor;
    int* edge_weights;
    int* sh_weights;
    short* node_levels;

    CHGraphExplorer(CHGraph* graph, TopologyAccessor accessor,
                    TopologyAccessor sh_accessor, int* edge_weights,
                    int* sh_weights, short* node_levels)
        : accessor(accessor), sh_accessor(sh_accessor)
    {
        this->graph = graph;
        this->edge_weights = edge_weights;
        this->sh_weights = sh_weights;
        this->node_levels = node_levels;
    }

    void forAdjacentEdges(int node, Direction dir, Adjacency typ,
                          std::function<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);
};

//*******************************************
// edge-ref iterators
//******************************************

class CHEdgeRefIterator : public IIterator<EdgeRef>
{
public:
    TopologyAccessor* accessor;
    TopologyAccessor* ch_accessor;
    char type;

    CHEdgeRefIterator(TopologyAccessor* accessor, TopologyAccessor* ch_accessor)
    {
        this->accessor = accessor;
        this->ch_accessor = ch_accessor;
        this->type = 0;
    }

    std::tuple<EdgeRef, bool> next();
};

class CHUpwardIterator : public IIterator<EdgeRef>
{
public:
    int this_node;
    short* node_levels;
    TopologyAccessor* accessor;
    TopologyAccessor* ch_accessor;
    char type;

    CHUpwardIterator(int this_node, short* levels, TopologyAccessor* accessor,
                     TopologyAccessor* ch_accessor)
    {
        this->this_node = this_node;
        this->node_levels = levels;
        this->accessor = accessor;
        this->ch_accessor = ch_accessor;
        this->type = 0;
    }

    std::tuple<EdgeRef, bool> next();
};

class CHDownwardIterator : public IIterator<EdgeRef>
{
public:
    int this_node;
    short* node_levels;
    TopologyAccessor* accessor;
    TopologyAccessor* ch_accessor;
    char type;

    CHDownwardIterator(int this_node, short* levels, TopologyAccessor* accessor,
                       TopologyAccessor* ch_accessor)
    {
        this->this_node = this_node;
        this->node_levels = levels;
        this->accessor = accessor;
        this->ch_accessor = ch_accessor;
        this->type = 0;
    }

    std::tuple<EdgeRef, bool> next();
};
