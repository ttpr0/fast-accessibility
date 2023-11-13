
#include "./ch_graph.h"

//*******************************************
// ch-graph
//*******************************************

std::unique_ptr<IGraphExplorer> CHGraph2::getGraphExplorer()
{
    return std::make_unique<CHGraphExplorer>(*this->base, *this->weights, *this->ch);
}
IGraphIndex& CHGraph2::getIndex()
{
    return *this->index;
}
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
    int m_node = this->ch->id_mapping.get_source(node);
    return this->base->getNode(m_node);
}
Edge CHGraph2::getEdge(int edge)
{
    Edge e = this->base->getEdge(edge);
    e.nodeA = this->ch->id_mapping.get_target(e.nodeA);
    e.nodeB = this->ch->id_mapping.get_target(e.nodeB);
    return e;
}
Coord CHGraph2::getNodeGeom(int node)
{
    int m_node = this->ch->id_mapping.get_source(node);
    return this->base->getNodeGeom(m_node);
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
const std::vector<CHEdge4>& CHGraph2::getDownEdges4(Direction dir)
{
    if (dir == Direction::FORWARD) {
        return this->ch_index->fwd_down_edges;
    } else {
        return this->ch_index->bwd_down_edges;
    }
}
short CHGraph2::getNodeTile(int node)
{
    int m_node = this->ch->id_mapping.get_source(node);
    return this->partition->get_node_tile(m_node);
}
int CHGraph2::tileCount()
{
    return this->partition->tile_count();
}
