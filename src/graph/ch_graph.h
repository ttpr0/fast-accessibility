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
    std::unique_ptr<IGraphIndex> index;

    // additional ch components
    CHStore ch_store;
    TopologyStore ch_topology;
    // stores all fowwards-down edges
    std::vector<CHEdge> fwd_down_edges;
    // stores all backwards-down edges
    std::vector<CHEdge> bwd_down_edges;

    CHGraph(GraphStore store, TopologyStore topology, std::vector<int> weights, CHStore ch_store, TopologyStore ch_topology);

    std::unique_ptr<IGraphExplorer> getGraphExplorer();
    IGraphIndex& getIndex();
    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);
    Coord getNodeGeom(int node);

    short getNodeLevel(int node);
    int shortcutCount();
    CHShortcut getShortcut(int shortcut);
    const std::vector<CHEdge>& getDownEdges(Direction dir);
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
    std::vector<int>& edge_weights;
    std::vector<int>& sh_weights;
    std::vector<short>& node_levels;

    CHGraphExplorer(CHGraph* graph, TopologyAccessor accessor, TopologyAccessor sh_accessor, std::vector<int>& edge_weights, std::vector<int>& sh_weights,
                    std::vector<short>& node_levels)
        : accessor(accessor), sh_accessor(sh_accessor), edge_weights(edge_weights), sh_weights(sh_weights), node_levels(node_levels)
    {
        this->graph = graph;
    }

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);
};
