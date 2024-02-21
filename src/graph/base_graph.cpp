
#include <math.h>

#include "./base/graph_index.h"
#include "./base_graph.h"

//*******************************************
// base-graph
//*******************************************

Graph build_base_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index)
{
    return {std::move(base), std::move(weights), std::move(index)};
}

Graph::Graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index)
    : base(std::move(base)), weights(std::move(weights)), index(std::move(index))
{}

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
int Graph::getClosestNode(Coord point)
{
    auto [id, ok] = this->index->getClosestNode(point);
    if (ok) {
        return id;
    }
    return -1;
}
void Graph::forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func)
{
    if (typ == Adjacency::ADJACENT_ALL || typ == Adjacency::ADJACENT_EDGES) {
        auto accessor = this->base->adjacency.getNeighbours(node, dir);
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
int Graph::getEdgeWeight(EdgeRef edge)
{
    return this->weights->get_edge_weight(edge.edge_id);
}
int Graph::getTurnCost(EdgeRef from, int via, EdgeRef to)
{
    return 0;
}
int Graph::getOtherNode(EdgeRef edge, int node)
{
    auto e = this->base->getEdge(edge.edge_id);
    if (node == e.nodeA) {
        return e.nodeB;
    }
    if (node == e.nodeB) {
        return e.nodeA;
    }
    return -1;
}

//*******************************************
// turn-cost-graph
//*******************************************

TCGraph build_tc_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<TCWeighting> weights, std::shared_ptr<IGraphIndex> index)
{
    return {std::move(base), std::move(weights), std::move(index)};
}

TCGraph::TCGraph(std::shared_ptr<GraphBase> base, std::shared_ptr<TCWeighting> weights, std::shared_ptr<IGraphIndex> index)
    : base(std::move(base)), weights(std::move(weights)), index(std::move(index))
{}

int TCGraph::nodeCount()
{
    return this->base->nodeCount();
}
int TCGraph::edgeCount()
{
    return this->base->edgeCount();
}
Node TCGraph::getNode(int node)
{
    return this->base->getNode(node);
}
Edge TCGraph::getEdge(int edge)
{
    return this->base->getEdge(edge);
}
Coord TCGraph::getNodeGeom(int node)
{
    return this->base->getNodeGeom(node);
}
int TCGraph::getClosestNode(Coord point)
{
    auto [id, ok] = this->index->getClosestNode(point);
    if (ok) {
        return id;
    }
    return -1;
}
void TCGraph::forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func)
{
    if (typ == Adjacency::ADJACENT_ALL || typ == Adjacency::ADJACENT_EDGES) {
        auto accessor = this->base->adjacency.getNeighbours(node, dir);
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
int TCGraph::getEdgeWeight(EdgeRef edge)
{
    return this->weights->get_edge_weight(edge.edge_id);
}
int TCGraph::getTurnCost(EdgeRef from, int via, EdgeRef to)
{
    return this->weights->get_turn_cost(from.edge_id, via, to.edge_id);
}
int TCGraph::getOtherNode(EdgeRef edge, int node)
{
    auto e = this->base->getEdge(edge.edge_id);
    if (node == e.nodeA) {
        return e.nodeB;
    }
    if (node == e.nodeB) {
        return e.nodeA;
    }
    return -1;
}
