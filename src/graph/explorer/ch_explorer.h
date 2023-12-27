#pragma once

#include <functional>
#include <vector>

#include "../comps/graph_base.h"
#include "../comps/id_mapping.h"
#include "../comps/weighting.h"
#include "../speed_ups/ch_data.h"
#include "../speed_ups/partition.h"
#include "../structs/structs.h"

//*******************************************
// ch-graph explorer
//*******************************************

class CHGraphExplorer
{
public:
    const GraphBase& base;
    const Weighting& weights;
    const CHData& ch;
    const _IDMapping& id_mapping;

    CHGraphExplorer(const GraphBase& base, const Weighting& weights, const CHData& ch, const _IDMapping& id_mapping) : base(base), weights(weights), ch(ch), id_mapping(id_mapping)
    {}

    int nodeCount();
    short getNodeLevel(int node);

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);
};
