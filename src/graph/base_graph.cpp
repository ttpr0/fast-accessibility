
#include <math.h>

#include "./base_graph.h"
#include "./comps/graph_index.h"

//*******************************************
// base-graph
//*******************************************

Graph build_base_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights)
{
    return {std::move(base), std::move(weights), std::make_unique<KDTreeIndex>(base->getKDTree())};
}

Graph::Graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::unique_ptr<IGraphIndex> index)
    : base(std::move(base)), weights(std::move(weights)), index(std::move(index))
{
    this->explorer = std::make_unique<BaseGraphExplorer>(*this->base, *this->weights);
}

IGraphExplorer& Graph::getGraphExplorer()
{
    return *this->explorer;
}
IGraphIndex& Graph::getIndex()
{
    return *this->index;
}
int Graph::nodeCount()
{
    return this->base->nodeCount();
}
int Graph::edgeCount()
{
    return this->base->edgeCount();
}
Node Graph::getNode(int node)
{
    return this->base->getNode(node);
}
Edge Graph::getEdge(int edge)
{
    return this->base->getEdge(edge);
}
Coord Graph::getNodeGeom(int node)
{
    return this->base->getNodeGeom(node);
}

//*******************************************
// base-graph explorer
//******************************************

void BaseGraphExplorer::forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func)
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
int BaseGraphExplorer::getEdgeWeight(EdgeRef edge)
{
    return this->weights.get_edge_weight(edge.edge_id);
}
int BaseGraphExplorer::getTurnCost(EdgeRef from, int via, EdgeRef to)
{
    return 0;
}
int BaseGraphExplorer::getOtherNode(EdgeRef edge, int node)
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
