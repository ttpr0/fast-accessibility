
#include "./ch_explorer.h"

//*******************************************
// ch-graph explorer
//******************************************

int CHGraphExplorer::nodeCount()
{
    return this->base.nodeCount();
}
short CHGraphExplorer::getNodeLevel(int node)
{
    return this->ch.getNodeLevel(node);
}
void CHGraphExplorer::forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func)
{
    if (typ == Adjacency::ADJACENT_ALL) {
        int m_node = this->id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        auto sh_accessor = this->ch.topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            int m_other_id = this->id_mapping.get_target(other_id);
            func(EdgeRef{edge_id, m_other_id, 0});
        }
        while (sh_accessor.next()) {
            int edge_id = sh_accessor.getEdgeID();
            int other_id = sh_accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 100});
        }
    } else if (typ == Adjacency::ADJACENT_EDGES) {
        int m_node = this->id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            int m_other_id = this->id_mapping.get_target(other_id);
            func(EdgeRef{edge_id, m_other_id, 0});
        }
    } else if (typ == Adjacency::ADJACENT_SHORTCUTS) {
        auto sh_accessor = this->ch.topology.getNeighbours(node, dir);
        while (sh_accessor.next()) {
            int edge_id = sh_accessor.getEdgeID();
            int other_id = sh_accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 0});
        }
    } else if (typ == Adjacency::ADJACENT_UPWARDS) {
        int m_node = this->id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        auto sh_accessor = this->ch.topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int other_id = accessor.getOtherID();
            int m_other_id = this->id_mapping.get_target(other_id);
            if (this->ch.node_levels[node] >= this->ch.node_levels[m_other_id]) {
                continue;
            }
            int edge_id = accessor.getEdgeID();
            func(EdgeRef{edge_id, m_other_id, 0});
        }
        while (sh_accessor.next()) {
            int other_id = sh_accessor.getOtherID();
            if (this->ch.node_levels[node] >= this->ch.node_levels[other_id]) {
                continue;
            }
            int edge_id = sh_accessor.getEdgeID();
            func(EdgeRef{edge_id, other_id, 100});
        }
    } else if (typ == Adjacency::ADJACENT_DOWNWARDS) {
        int m_node = this->id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        auto sh_accessor = this->ch.topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int other_id = accessor.getOtherID();
            int m_other_id = this->id_mapping.get_target(other_id);
            if (this->ch.node_levels[node] <= this->ch.node_levels[m_other_id]) {
                continue;
            }
            int edge_id = accessor.getEdgeID();
            func(EdgeRef{edge_id, m_other_id, 0});
        }
        while (sh_accessor.next()) {
            int other_id = sh_accessor.getOtherID();
            if (this->ch.node_levels[node] <= this->ch.node_levels[other_id]) {
                continue;
            }
            int edge_id = sh_accessor.getEdgeID();
            func(EdgeRef{edge_id, other_id, 100});
        }
    } else {
        throw 1;
    }
    return;
}
int CHGraphExplorer::getEdgeWeight(EdgeRef edge)
{
    if (edge.isCHShortcut()) {
        auto shc = this->ch.getShortcut(edge.edge_id);
        return shc.weight;
    } else {
        return this->weights.get_edge_weight(edge.edge_id);
    }
}
int CHGraphExplorer::getTurnCost(EdgeRef from, int via, EdgeRef to)
{
    return 0;
}
int CHGraphExplorer::getOtherNode(EdgeRef edge, int node)
{
    if (edge.isShortcut()) {
        auto e = this->ch.getShortcut(edge.edge_id);
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
