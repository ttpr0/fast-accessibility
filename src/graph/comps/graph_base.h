#pragma once

#include <memory>
#include <vector>

#include "../../kd_tree/kd_tree.h"
#include "../structs/adjacency.h"
#include "../structs/geom.h"
#include "../structs/structs.h"

struct GraphBase
{
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<Coord> node_geoms;
    AdjacencyArray adjacency;
    KDTree tree;

    inline int nodeCount() { return this->nodes.size(); }
    inline int edgeCount() { return this->edges.size(); }
    inline Node getNode(int node) { return this->nodes[node]; }
    inline Edge getEdge(int edge) { return this->edges[edge]; }
    inline Coord getNodeGeom(int node) { return this->node_geoms[node]; }
    KDTree& getKDTree() { return this->tree; }
};

std::shared_ptr<GraphBase> build_graph_base(std::vector<Node> nodes, std::vector<Edge> edges, std::vector<Coord> node_geoms);
