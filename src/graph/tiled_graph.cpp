
#include "./tiled_graph.h"

//*******************************************
// base-graph
//******************************************

TiledGraph::TiledGraph(GraphStore store, TopologyStore topology, std::vector<int> weights, TiledStore skip_store, TypedTopologyStore skip_topology,
                       std::unordered_map<short, std::tuple<int, int>> tile_ranges, std::vector<TiledSHEdge> index_edges)
    : store(store), topology(topology), edge_weights(weights), skip_store(skip_store), skip_topology(skip_topology), tile_ranges(tile_ranges), index_edges(index_edges)
{
    this->index = build_kdtree_index(store.node_geoms);
}

std::unique_ptr<IGraphExplorer> TiledGraph::getGraphExplorer()
{
    return std::make_unique<TiledGraphExplorer>(this, this->topology.getAccessor(), this->skip_topology.getAccessor(), this->edge_weights, this->skip_store.sh_weights);
}
IGraphIndex& TiledGraph::getIndex()
{
    return *this->index;
}
int TiledGraph::nodeCount()
{
    return this->store.nodeCount();
}
int TiledGraph::edgeCount()
{
    return this->store.edgeCount();
}
Node TiledGraph::getNode(int node)
{
    return this->store.getNode(node);
}
Edge TiledGraph::getEdge(int edge)
{
    return this->store.getEdge(edge);
}
Coord TiledGraph::getNodeGeom(int node)
{
    return this->store.getNodeGeom(node);
}

short TiledGraph::getNodeTile(int node)
{
    return this->skip_store.getNodeTile(node);
}
short TiledGraph::tileCount()
{
    return this->skip_store.tileCount();
}
int TiledGraph::shortcutCount()
{
    return this->skip_store.shortcutCount();
}
Shortcut TiledGraph::getShortcut(int shortcut)
{
    return this->skip_store.getShortcut(shortcut);
}
const std::span<TiledSHEdge> TiledGraph::getIndexEdges(short tile, Direction dir)
{
    auto [s, c] = this->tile_ranges[tile];
    return std::span<TiledSHEdge>(&this->index_edges[s], c);
}

//*******************************************
// ch-graph explorer
//*******************************************

void TiledGraphExplorer::forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func)
{
    if (typ == ADJACENT_SKIP) {
        this->skip_accessor.setBaseNode(node, dir);
        while (this->skip_accessor.next()) {
            int edge_id = this->skip_accessor.getEdgeID();
            int other_id = this->skip_accessor.getOtherID();
            char typ = this->skip_accessor.getType();
            func({edge_id, other_id, typ});
        }
    } else if (typ == ADJACENT_ALL || typ == ADJACENT_EDGES) {
        this->accessor.setBaseNode(node, dir);
        while (this->accessor.next()) {
            int edge_id = this->accessor.getEdgeID();
            int other_id = this->accessor.getOtherID();
            char typ = this->graph->skip_store.edge_types[edge_id];
            func({edge_id, other_id, typ});
        }
    } else {
        throw 1;
    }
    return;
}
int TiledGraphExplorer::getEdgeWeight(EdgeRef edge)
{
    if (edge.isShortcut()) {
        return this->sh_weights[edge.edge_id];
    } else {
        return this->edge_weights[edge.edge_id];
    }
}
int TiledGraphExplorer::getTurnCost(EdgeRef from, int via, EdgeRef to)
{
    return 0;
}
int TiledGraphExplorer::getOtherNode(EdgeRef edge, int node)
{
    if (edge.isShortcut()) {
        auto e = this->graph->getShortcut(edge.edge_id);
        if (node == e.nodeA) {
            return e.nodeB;
        }
        if (node == e.nodeB) {
            return e.nodeA;
        }
        return -1;
    } else {
        auto e = this->graph->getEdge(edge.edge_id);
        if (node == e.nodeA) {
            return e.nodeB;
        }
        if (node == e.nodeB) {
            return e.nodeA;
        }
        return -1;
    }
}
