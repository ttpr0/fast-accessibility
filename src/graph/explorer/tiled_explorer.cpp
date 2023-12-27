
#include "./tiled_explorer.h"

//*******************************************
// ch-graph explorer
//*******************************************

int TiledGraphExplorer::nodeCount()
{
    return this->base.nodeCount();
}
short TiledGraphExplorer::getNodeTile(int node)
{
    int m_node = this->id_mapping.get_source(node);
    return this->partition.get_node_tile(m_node);
}
void TiledGraphExplorer::forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func)
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
int TiledGraphExplorer::getEdgeWeight(EdgeRef edge)
{
    if (edge.isShortcut()) {
        auto shc = this->tiled.getShortcut(edge.edge_id);
        return shc.weight;
    } else {
        return this->weights.get_edge_weight(edge.edge_id);
    }
}
int TiledGraphExplorer::getTurnCost(EdgeRef from, int via, EdgeRef to)
{
    return 0;
}
int TiledGraphExplorer::getOtherNode(EdgeRef edge, int node)
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