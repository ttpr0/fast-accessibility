#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "./base_graph.h"
#include "./ch_storage.h"
#include "./graph.h"
#include "./graph_storage.h"
#include "./tiled_storage.h"
#include "./topology_storage.h"

//*******************************************
// base-graph
//******************************************

class TiledGraph : public ITiledGraph
{
public:
    GraphStore store;
    TopologyStore topology;
    std::vector<int> edge_weights;
    std::unique_ptr<IGraphIndex> index;

    // additional components
    TiledStore skip_store;
    TypedTopologyStore skip_topology;
    // cell-index
    std::unordered_map<short, std::tuple<int, int>> tile_ranges;
    std::vector<TiledSHEdge> index_edges;

    TiledGraph(GraphStore store, TopologyStore topology, std::vector<int> weights, TiledStore skip_store, TypedTopologyStore skip_topology,
               std::unordered_map<short, std::tuple<int, int>> tile_ranges, std::vector<TiledSHEdge> index_edges);

    std::unique_ptr<IGraphExplorer> getGraphExplorer();
    IGraphIndex& getIndex();
    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);
    Coord getNodeGeom(int node);

    short getNodeTile(int node);
    short tileCount();
    int shortcutCount();
    Shortcut getShortcut(int shortcut);
    const std::span<TiledSHEdge> getIndexEdges(short tile, Direction dir);
};

//*******************************************
// ch-graph explorer
//******************************************

class TiledGraphExplorer : public IGraphExplorer
{
public:
    TiledGraph* graph;
    TopologyAccessor accessor;
    TypedTopologyAccessor skip_accessor;
    std::vector<int>& edge_weights;
    std::vector<int>& sh_weights;

    TiledGraphExplorer(TiledGraph* graph, TopologyAccessor accessor, TypedTopologyAccessor skip_accessor, std::vector<int>& edge_weights, std::vector<int>& sh_weights)
        : accessor(accessor), skip_accessor(skip_accessor), edge_weights(edge_weights), sh_weights(sh_weights)
    {
        this->graph = graph;
    }

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);
};