#pragma once

#include <functional>
#include <vector>

#include "../comps/graph_base.h"
#include "../comps/weighting.h"
#include "../structs/structs.h"

//*******************************************
// base-graph explorer
//******************************************

class BaseGraphExplorer
{
public:
    const GraphBase& base;
    const Weighting& weights;

    BaseGraphExplorer(const GraphBase& base, const Weighting& weights) : base(base), weights(weights) {}

    int nodeCount();
    int edgeCount();

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);

    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);

    int getOtherNode(EdgeRef edge, int node);
};
