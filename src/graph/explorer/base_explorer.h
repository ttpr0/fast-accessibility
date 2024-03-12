#pragma once

#include <functional>
#include <vector>

#include "../../util/function_ref.h"
#include "../base/graph_base.h"
#include "../structs/structs.h"
#include "../weights/weighting.h"

//*******************************************
// base-graph explorer
//******************************************

class BaseGraphExplorer
{
public:
    const GraphBase& base;
    const Weighting& weights;

    BaseGraphExplorer(const GraphBase& base, const Weighting& weights) : base(base), weights(weights) {}

    int nodeCount() const;
    int edgeCount() const;

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func) const;

    int getEdgeWeight(EdgeRef edge) const;
    int getTurnCost(EdgeRef from, int via, EdgeRef to) const;

    int getOtherNode(EdgeRef edge, int node) const;
};
