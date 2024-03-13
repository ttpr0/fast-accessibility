#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "../util/function_ref.h"
#include "./base/graph_base.h"
#include "./base/graph_index.h"
#include "./graph.h"
#include "./structs/adjacency.h"
#include "./weights/tc_weighting.h"
#include "./weights/weighting.h"

//*******************************************
// base-graph
//*******************************************

class Graph : public IGraph
{
public:
    std::shared_ptr<GraphBase> base;
    std::shared_ptr<Weighting> weights;

    Graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights) : base(std::move(base)), weights(std::move(weights)) {}

    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);

    Coord getNodeGeom(int node);

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);
};

Graph build_base_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> edge_weights);

//*******************************************
// turn-cost-graph
//*******************************************

class TCGraph : public IGraph
{
public:
    std::shared_ptr<GraphBase> base;
    std::shared_ptr<TCWeighting> weights;

    TCGraph(std::shared_ptr<GraphBase> base, std::shared_ptr<TCWeighting> weights) : base(std::move(base)), weights(std::move(weights)) {}

    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);

    Coord getNodeGeom(int node);

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);
};

TCGraph build_tc_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<TCWeighting> weights);
