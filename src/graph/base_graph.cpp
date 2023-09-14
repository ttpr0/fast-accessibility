
#include <math.h>

#include "./base_graph.h"

//*******************************************
// base-graph
//*******************************************

std::unique_ptr<IGraphExplorer> Graph::getGraphExplorer()
{
    return std::make_unique<BaseGraphExplorer>(this, this->topology.getAccessor(), this->edge_weights);
}
std::unique_ptr<IGraphIndex> Graph::getIndex()
{
    return std::make_unique<BaseGraphIndex>(this->store.node_geoms);
}
int Graph::nodeCount()
{
    return this->store.nodeCount();
}
int Graph::edgeCount()
{
    return this->store.edgeCount();
}
Node Graph::getNode(int node)
{
    return this->store.getNode(node);
}
Edge Graph::getEdge(int edge)
{
    return this->store.getEdge(edge);
}
Coord Graph::getNodeGeom(int node)
{
    return this->store.getNodeGeom(node);
}

//*******************************************
// base-graph explorer
//******************************************

void BaseGraphExplorer::forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func)
{
    if (typ == Adjacency::ADJACENT_ALL || typ == Adjacency::ADJACENT_EDGES) {
        this->accessor.setBaseNode(node, dir);
        while (this->accessor.next()) {
            int edge_id = this->accessor.getEdgeID();
            int other_id = this->accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 0});
        }
    } else {
        throw 1;
    }
    return;
}
int BaseGraphExplorer::getEdgeWeight(EdgeRef edge)
{
    return this->edge_weights[edge.edge_id];
}
int BaseGraphExplorer::getTurnCost(EdgeRef from, int via, EdgeRef to)
{
    return 0;
}
int BaseGraphExplorer::getOtherNode(EdgeRef edge, int node)
{
    auto e = this->graph->getEdge(edge.edge_id);
    if (node == e.nodeA) {
        return e.nodeB;
    }
    if (node == e.nodeB) {
        return e.nodeA;
    }
    return -1;
}

//*******************************************
// graph index
//*******************************************

int BaseGraphIndex::getClosestNode(Coord point)
{
    int closest = -1;
    int min_dist = 100000;
    for (int i = 0; i < this->node_geoms.size(); i++) {
        Coord other = this->node_geoms[i];
        float d_lon = other.lon - point.lon;
        float d_lat = other.lat - point.lat;
        float dist = sqrt(d_lon * d_lon + d_lat * d_lat);
        if (dist < min_dist) {
            min_dist = dist;
            closest = i;
        }
    }
    return closest;
}
