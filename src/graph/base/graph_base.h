#pragma once

#include <memory>
#include <vector>

#include "../structs/adjacency.h"
#include "../structs/geom.h"
#include "../structs/structs.h"

struct GraphBase
{
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    AdjacencyArray adjacency;

    inline int nodeCount() const { return this->nodes.size(); }
    inline int edgeCount() const { return this->edges.size(); }
    inline Node getNode(int node) const { return this->nodes[node]; }
    inline Edge getEdge(int edge) const { return this->edges[edge]; }
    inline Coord getNodeGeom(int node) const { return this->nodes[node].location; }
};

std::shared_ptr<GraphBase> new_graph_base(std::vector<Node> nodes, std::vector<Edge> edges);
