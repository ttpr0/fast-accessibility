
#include <math.h>

#include "./base_graph.h"

//*******************************************
// base-graph
//*******************************************

Graph::Graph(GraphStore store, TopologyStore topology, std::vector<int> weights) : store(store), topology(topology), edge_weights(weights)
{
    this->index = build_kdtree_index(store.node_geoms);
}

std::unique_ptr<IGraphExplorer> Graph::getGraphExplorer()
{
    return std::make_unique<BaseGraphExplorer>(this, this->topology.getAccessor(), this->edge_weights);
}
IGraphIndex& Graph::getIndex()
{
    return *this->index;
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

// KDTreeIndex::KDTreeIndex(std::vector<Coord>& node_geoms)
// {
//     this->tree = KDTree();
//     for (int i = 0; i < node_geoms.size(); i++) {
//         auto coord = node_geoms[i];
//         this->tree.insert(coord.lon, coord.lat, i);
//     }
// }
KDTreeIndex::KDTreeIndex(std::vector<Coord>& node_geoms)
{
    this->tree = KDTree();
    std::vector<TreeValue> values(node_geoms.size());
    for (int i = 0; i < node_geoms.size(); i++) {
        auto coord = node_geoms[i];
        values[i] = {{coord.lon, coord.lat}, i};
    }
    this->tree.create_balanced(values);
}

int KDTreeIndex::getClosestNode(Coord point)
{
    auto [id, ok] = this->tree.get_closest(point.lon, point.lat, 0.01);
    if (ok) {
        return id;
    }
    return -1;
}

std::unique_ptr<IGraphIndex> build_kdtree_index(std::vector<Coord>& node_geoms)
{
    return std::make_unique<KDTreeIndex>(node_geoms);
}
