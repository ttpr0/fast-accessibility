
#include "./ch_graph.h"

//*******************************************
// ch-graph
//*******************************************

CHGraph2::CHGraph2(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index, std::shared_ptr<Partition> partition,
                   std::shared_ptr<_IDMapping> id_mapping, std::shared_ptr<CHData> ch, std::shared_ptr<_CHIndex> ch_index)
    : base(std::move(base)),
      weights(std::move(weights)),
      index(std::move(index)),
      partition(std::move(partition)),
      id_mapping(std::move(id_mapping)),
      ch(std::move(ch)),
      ch_index(std::move(ch_index))
{}

int CHGraph2::nodeCount()
{
    return this->base->nodeCount();
}
int CHGraph2::edgeCount()
{
    return this->base->edgeCount();
}
Node CHGraph2::getNode(int node)
{
    int m_node = this->id_mapping->get_source(node);
    return this->base->getNode(m_node);
}
Edge CHGraph2::getEdge(int edge)
{
    Edge e = this->base->getEdge(edge);
    e.nodeA = this->id_mapping->get_target(e.nodeA);
    e.nodeB = this->id_mapping->get_target(e.nodeB);
    return e;
}
Coord CHGraph2::getNodeGeom(int node)
{
    int m_node = this->id_mapping->get_source(node);
    return this->base->getNodeGeom(m_node);
}
int CHGraph2::getClosestNode(Coord point)
{
    auto [id, ok] = this->index->getClosestNode(point);
    if (ok) {
        int m_id = this->id_mapping->get_target(id);
        return m_id;
    }
    return -1;
}

void CHGraph2::forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func)
{
    if (typ == Adjacency::ADJACENT_ALL) {
        int m_node = this->id_mapping->get_source(node);
        auto accessor = this->base->adjacency.getNeighbours(m_node, dir);
        auto sh_accessor = this->ch->topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            int m_other_id = this->id_mapping->get_target(other_id);
            func(EdgeRef{edge_id, m_other_id, 0});
        }
        while (sh_accessor.next()) {
            int edge_id = sh_accessor.getEdgeID();
            int other_id = sh_accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 100});
        }
    } else if (typ == Adjacency::ADJACENT_EDGES) {
        int m_node = this->id_mapping->get_source(node);
        auto accessor = this->base->adjacency.getNeighbours(m_node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            int m_other_id = this->id_mapping->get_target(other_id);
            func(EdgeRef{edge_id, m_other_id, 0});
        }
    } else if (typ == Adjacency::ADJACENT_SHORTCUTS) {
        auto sh_accessor = this->ch->topology.getNeighbours(node, dir);
        while (sh_accessor.next()) {
            int edge_id = sh_accessor.getEdgeID();
            int other_id = sh_accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 0});
        }
    } else if (typ == Adjacency::ADJACENT_UPWARDS) {
        int m_node = this->id_mapping->get_source(node);
        auto accessor = this->base->adjacency.getNeighbours(m_node, dir);
        auto sh_accessor = this->ch->topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int other_id = accessor.getOtherID();
            int m_other_id = this->id_mapping->get_target(other_id);
            if (this->ch->node_levels[node] >= this->ch->node_levels[m_other_id]) {
                continue;
            }
            int edge_id = accessor.getEdgeID();
            func(EdgeRef{edge_id, m_other_id, 0});
        }
        while (sh_accessor.next()) {
            int other_id = sh_accessor.getOtherID();
            if (this->ch->node_levels[node] >= this->ch->node_levels[other_id]) {
                continue;
            }
            int edge_id = sh_accessor.getEdgeID();
            func(EdgeRef{edge_id, other_id, 100});
        }
    } else if (typ == Adjacency::ADJACENT_DOWNWARDS) {
        int m_node = this->id_mapping->get_source(node);
        auto accessor = this->base->adjacency.getNeighbours(m_node, dir);
        auto sh_accessor = this->ch->topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int other_id = accessor.getOtherID();
            int m_other_id = this->id_mapping->get_target(other_id);
            if (this->ch->node_levels[node] <= this->ch->node_levels[m_other_id]) {
                continue;
            }
            int edge_id = accessor.getEdgeID();
            func(EdgeRef{edge_id, m_other_id, 0});
        }
        while (sh_accessor.next()) {
            int other_id = sh_accessor.getOtherID();
            if (this->ch->node_levels[node] <= this->ch->node_levels[other_id]) {
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
int CHGraph2::getEdgeWeight(EdgeRef edge)
{
    if (edge.isCHShortcut()) {
        auto shc = this->ch->getShortcut(edge.edge_id);
        return shc.weight;
    } else {
        return this->weights->get_edge_weight(edge.edge_id);
    }
}
int CHGraph2::getTurnCost(EdgeRef from, int via, EdgeRef to)
{
    return 0;
}
int CHGraph2::getOtherNode(EdgeRef edge, int node)
{
    if (edge.isShortcut()) {
        auto e = this->ch->getShortcut(edge.edge_id);
        if (node == e.from) {
            return e.to;
        }
        if (node == e.to) {
            return e.from;
        }
        return -1;
    } else {
        auto e = this->base->getEdge(edge.edge_id);
        int m_node_a = this->id_mapping->get_target(e.nodeA);
        int m_node_b = this->id_mapping->get_target(e.nodeB);
        if (node == m_node_a) {
            return m_node_b;
        }
        if (node == m_node_b) {
            return m_node_a;
        }
        return -1;
    }
}

short CHGraph2::getNodeLevel(int node)
{
    return this->ch->getNodeLevel(node);
}
int CHGraph2::shortcutCount()
{
    return this->ch->shortcutCount();
}
Shortcut CHGraph2::getShortcut(int shortcut)
{
    return this->ch->getShortcut(shortcut);
}
const std::vector<Shortcut>& CHGraph2::getDownEdges(Direction dir)
{
    if (dir == Direction::FORWARD) {
        return this->ch_index->get_fwd_index();
    } else {
        return this->ch_index->get_bwd_index();
    }
}
short CHGraph2::getNodeTile(int node)
{
    int m_node = this->id_mapping->get_source(node);
    return this->partition->get_node_tile(m_node);
}
int CHGraph2::tileCount()
{
    return this->partition->tile_count();
}
