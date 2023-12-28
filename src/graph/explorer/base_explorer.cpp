
#include "./base_explorer.h"

//*******************************************
// base-graph explorer
//******************************************

int BaseGraphExplorer::nodeCount() const
{
    return this->base.nodeCount();
}
int BaseGraphExplorer::edgeCount() const
{
    return this->base.edgeCount();
}
void BaseGraphExplorer::forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func) const
{
    if (typ == Adjacency::ADJACENT_ALL || typ == Adjacency::ADJACENT_EDGES) {
        auto accessor = this->base.adjacency.getNeighbours(node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 0});
        }
    } else {
        throw 1;
    }
    return;
}
int BaseGraphExplorer::getEdgeWeight(EdgeRef edge) const
{
    return this->weights.get_edge_weight(edge.edge_id);
}
int BaseGraphExplorer::getTurnCost(EdgeRef from, int via, EdgeRef to) const
{
    return 0;
}
int BaseGraphExplorer::getOtherNode(EdgeRef edge, int node) const
{
    auto e = this->base.getEdge(edge.edge_id);
    if (node == e.nodeA) {
        return e.nodeB;
    }
    if (node == e.nodeB) {
        return e.nodeA;
    }
    return -1;
}
