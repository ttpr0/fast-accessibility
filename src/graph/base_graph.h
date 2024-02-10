#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "./base/graph_base.h"
#include "./base/graph_index.h"
#include "./graph.h"
#include "./structs/adjacency.h"
#include "./weights/weighting.h"

//*******************************************
// base-graph
//*******************************************

class Graph : public IGraph
{
public:
    std::shared_ptr<GraphBase> base;
    std::shared_ptr<Weighting> weights;
    std::shared_ptr<IGraphIndex> index;

    Graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index);

    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);

    Coord getNodeGeom(int node);
    int getClosestNode(Coord point);

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);
};

Graph build_base_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> edge_weights, std::shared_ptr<IGraphIndex> index);
