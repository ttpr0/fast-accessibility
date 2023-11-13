
#include <array>

#include "./comps/graph_index.h"
#include "./tiled_graph.h"

//*******************************************
// tiled-graph
//******************************************

TiledGraph build_tiled_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights,
                             std::shared_ptr<Partition> partition, std::shared_ptr<TiledData> tiled,
                             std::shared_ptr<_CellIndex> index)
{
    return {std::move(base),
            std::move(weights),
            std::make_unique<MappedKDTreeIndex>(base->getKDTree(), tiled->id_mapping),
            std::move(partition),
            std::move(tiled),
            std::move(index)};
}

std::unique_ptr<IGraphExplorer> TiledGraph::getGraphExplorer()
{
    return std::make_unique<TiledGraphExplorer>(*this->base, *this->weights, *this->tiled);
}
IGraphIndex& TiledGraph::getIndex()
{
    return *this->index;
}
int TiledGraph::nodeCount()
{
    return this->base->nodeCount();
}
int TiledGraph::edgeCount()
{
    return this->base->edgeCount();
}
Node TiledGraph::getNode(int node)
{
    int m_node = this->tiled->id_mapping.get_source(node);
    return this->base->getNode(m_node);
}
Edge TiledGraph::getEdge(int edge)
{
    Edge e = this->base->getEdge(edge);
    e.nodeA = this->tiled->id_mapping.get_target(e.nodeA);
    e.nodeB = this->tiled->id_mapping.get_target(e.nodeB);
    return e;
}
Coord TiledGraph::getNodeGeom(int node)
{
    int m_node = this->tiled->id_mapping.get_source(node);
    return this->base->getNodeGeom(m_node);
}

short TiledGraph::getNodeTile(int node)
{
    int m_node = this->tiled->id_mapping.get_source(node);
    return this->partition->get_node_tile(m_node);
}
short TiledGraph::tileCount()
{
    return this->partition->tile_count();
}
int TiledGraph::shortcutCount()
{
    return this->tiled->shortcutCount();
}
Shortcut TiledGraph::getShortcut(int shortcut)
{
    return this->tiled->getShortcut(shortcut);
}
const std::span<Shortcut> TiledGraph::getIndexEdges(short tile, Direction dir)
{
    if (dir == Direction::FORWARD) {
        auto& edges = this->cell_index->get_fwd_index(tile);
        return std::span<Shortcut>(edges.data(), edges.size());
    } else {
        auto& edges = this->cell_index->get_bwd_index(tile);
        return std::span<Shortcut>(edges.data(), edges.size());
    }
}

//*******************************************
// ch-graph explorer
//*******************************************

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
        int m_node = this->tiled.id_mapping.get_source(node);
        auto accessor = this->base.adjacency.getNeighbours(m_node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            int m_other_id = this->tiled.id_mapping.get_target(node);
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
        int m_node_a = this->tiled.id_mapping.get_target(e.nodeA);
        int m_node_b = this->tiled.id_mapping.get_target(e.nodeB);
        if (node == m_node_a) {
            return m_node_b;
        }
        if (node == m_node_b) {
            return m_node_a;
        }
        return -1;
    }
}
