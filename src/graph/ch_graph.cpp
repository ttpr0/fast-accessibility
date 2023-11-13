
#include "./ch_graph.h"
#include "./comps/graph_index.h"

//*******************************************
// base-graph
//*******************************************

CHGraph build_ch_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<CHData> ch,
                       std::shared_ptr<_CHIndex> ch_index)
{
    return {std::move(base), std::move(weights), std::make_unique<MappedKDTreeIndex>(base->getKDTree(), ch->id_mapping),
            std::move(ch), std::move(ch_index)};
}

CHGraph2 build_ch_graph_2(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights,
                          std::shared_ptr<Partition> partition, std::shared_ptr<CHData> ch,
                          std::shared_ptr<_CHIndex2> ch_index)
{
    return {std::move(base),
            std::move(weights),
            std::make_unique<MappedKDTreeIndex>(base->getKDTree(), ch->id_mapping),
            std::move(partition),
            std::move(ch),
            std::move(ch_index)};
}

std::unique_ptr<IGraphExplorer> CHGraph::getGraphExplorer()
{
    return std::make_unique<CHGraphExplorer>(*this->base, *this->weights, *this->ch);
}
IGraphIndex& CHGraph::getIndex()
{
    return *this->index;
}
int CHGraph::nodeCount()
{
    return this->base->nodeCount();
}
int CHGraph::edgeCount()
{
    return this->base->edgeCount();
}
Node CHGraph::getNode(int node)
{
    int m_node = this->ch->id_mapping.get_source(node);
    return this->base->getNode(m_node);
}
Edge CHGraph::getEdge(int edge)
{
    Edge e = this->base->getEdge(edge);
    e.nodeA = this->ch->id_mapping.get_target(e.nodeA);
    e.nodeB = this->ch->id_mapping.get_target(e.nodeB);
    return e;
}
Coord CHGraph::getNodeGeom(int node)
{
    int m_node = this->ch->id_mapping.get_source(node);
    return this->base->getNodeGeom(m_node);
}

short CHGraph::getNodeLevel(int node)
{
    return this->ch->getNodeLevel(node);
}
int CHGraph::shortcutCount()
{
    return this->ch->shortcutCount();
}
Shortcut CHGraph::getShortcut(int shortcut)
{
    return this->ch->getShortcut(shortcut);
}
const std::vector<CHEdge>& CHGraph::getDownEdges(Direction dir)
{
    if (dir == Direction::FORWARD) {
        return this->ch_index->fwd_down_edges;
    } else {
        return this->ch_index->bwd_down_edges;
    }
}

//*******************************************
// ch-graph explorer
//******************************************

void CHGraphExplorer::forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func)
{
    if (typ == Adjacency::ADJACENT_ALL) {
        int m_node = this->ch.id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        auto sh_accessor = this->ch.topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            int m_other_id = this->ch.id_mapping.get_target(other_id);
            func(EdgeRef{edge_id, m_other_id, 0});
        }
        while (sh_accessor.next()) {
            int edge_id = sh_accessor.getEdgeID();
            int other_id = sh_accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 100});
        }
    } else if (typ == Adjacency::ADJACENT_EDGES) {
        int m_node = this->ch.id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            int m_other_id = this->ch.id_mapping.get_target(other_id);
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
        int m_node = this->ch.id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        auto sh_accessor = this->ch.topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int other_id = accessor.getOtherID();
            int m_other_id = this->ch.id_mapping.get_target(other_id);
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
        int m_node = this->ch.id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        auto sh_accessor = this->ch.topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int other_id = accessor.getOtherID();
            int m_other_id = this->ch.id_mapping.get_target(other_id);
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
        int m_node_a = this->ch.id_mapping.get_target(e.nodeA);
        int m_node_b = this->ch.id_mapping.get_target(e.nodeB);
        if (node == m_node_a) {
            return m_node_b;
        }
        if (node == m_node_b) {
            return m_node_a;
        }
        return -1;
    }
}
