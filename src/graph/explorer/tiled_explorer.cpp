
#include "./tiled_explorer.h"

//*******************************************
// ch-graph explorer
//*******************************************

int TiledGraphExplorer::nodeCount() const
{
    return this->base.nodeCount();
}
int TiledGraphExplorer::edgeCount() const
{
    return this->base.edgeCount();
}
Node TiledGraphExplorer::getNode(int node) const
{
    int m_node = this->id_mapping.get_target(node);
    return this->base.getNode(m_node);
}
Edge TiledGraphExplorer::getEdge(int edge) const
{
    Edge e = this->base.getEdge(edge);
    e.nodeA = this->id_mapping.get_target(e.nodeA);
    e.nodeB = this->id_mapping.get_target(e.nodeB);
    return e;
}
Coord TiledGraphExplorer::getNodeGeom(int node) const
{
    int m_node = this->id_mapping.get_target(node);
    return this->base.getNodeGeom(m_node);
}
int TiledGraphExplorer::shortcutCount() const
{
    return this->tiled.shortcutCount();
}
Shortcut TiledGraphExplorer::getShortcut(int shortcut) const
{
    return this->tiled.getShortcut(shortcut);
}
short TiledGraphExplorer::getNodeTile(int node) const
{
    int m_node = this->id_mapping.get_source(node);
    return this->partition.get_node_tile(m_node);
}
short TiledGraphExplorer::tileCount() const
{
    return this->partition.tile_count();
}
void TiledGraphExplorer::forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func) const
{
    if (typ == ADJACENT_SKIP) {
        auto skip_accessor = this->tiled.topology.getNeighbours(node, dir);
        while (skip_accessor.next()) {
            int edge_id = skip_accessor.getEdgeID();
            int other_id = skip_accessor.getOtherID();
            std::array<char, 8> data = skip_accessor.getData();
            func({edge_id, other_id, data[0]});
        }
    } else if (typ == ADJACENT_ALL || typ == ADJACENT_EDGES) {
        int m_node = this->id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            int m_other_id = this->id_mapping.get_target(other_id);
            char typ = this->tiled.edge_types[edge_id];
            func({edge_id, m_other_id, typ});
        }
    } else {
        throw 1;
    }
    return;
}
int TiledGraphExplorer::getEdgeWeight(EdgeRef edge) const
{
    if (edge.isShortcut()) {
        auto shc = this->tiled.getShortcut(edge.edge_id);
        return shc.weight;
    } else {
        return this->weights.get_edge_weight(edge.edge_id);
    }
}
int TiledGraphExplorer::getOtherNode(EdgeRef edge, int node) const
{
    if (edge.isShortcut()) {
        auto e = this->tiled.getShortcut(edge.edge_id);
        if (node == e.from) {
            return e.to;
        }
        if (node == e.to) {
            return e.from;
        }
        return -1;
    } else {
        auto e = this->base.getEdge(edge.edge_id);
        int m_node_a = this->id_mapping.get_target(e.nodeA);
        int m_node_b = this->id_mapping.get_target(e.nodeB);
        if (node == m_node_a) {
            return m_node_b;
        }
        if (node == m_node_b) {
            return m_node_a;
        }
        return -1;
    }
}
