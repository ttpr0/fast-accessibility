#pragma once

#include <vector>

#include "./structs.h"
#include "./geom.h"

class GraphStore
{
public:
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<Coord> node_geoms;

    GraphStore(std::vector<Node> nodes, std::vector<Edge> edges, std::vector<Coord> node_geoms)
        : nodes(nodes), edges(edges), node_geoms(node_geoms)
    {}

    inline int nodeCount() {
        return this->nodes.size();
    }
    inline int edgeCount() {
        return this->edges.size();
    }
    inline Node getNode(int node) {
        return this->nodes[node];
    }
    inline Edge getEdge(int edge) {
        return this->edges[edge];
    }
    inline Coord getNodeGeom(int node) {
        return this->node_geoms[node];
    }
};
