#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "../kd_tree/kd_tree.h"
#include "./comps/graph_base.h"
#include "./comps/weighting.h"
#include "./graph.h"
#include "./structs/adjacency.h"

//*******************************************
// classes
//*******************************************

class Graph;
class BaseGraphExplorer;
class BaseGraphIndex;
class KDTreeIndex;

//*******************************************
// base-graph
//*******************************************

Graph build_base_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> edge_weights);

class Graph : public IGraph
{
public:
    std::shared_ptr<GraphBase> base;
    std::shared_ptr<Weighting> weights;
    std::unique_ptr<IGraphIndex> index;

    Graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::unique_ptr<IGraphIndex> index)
        : base(std::move(base)), weights(std::move(weights)), index(std::move(index))
    {}

    std::unique_ptr<IGraphExplorer> getGraphExplorer();

    IGraphIndex& getIndex();

    int nodeCount();
    int edgeCount();

    Node getNode(int node);
    Edge getEdge(int edge);

    Coord getNodeGeom(int node);
};

//*******************************************
// base-graph explorer
//******************************************

class BaseGraphExplorer : public IGraphExplorer
{
public:
    GraphBase& base;
    Weighting& weights;

    BaseGraphExplorer(GraphBase& base, Weighting& weights) : base(base), weights(weights) {}

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);

    int getEdgeWeight(EdgeRef edge);

    int getTurnCost(EdgeRef from, int via, EdgeRef to);

    int getOtherNode(EdgeRef edge, int node);
};
