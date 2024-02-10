#pragma once

#include <functional>
#include <vector>

#include "../base/graph_base.h"
#include "../base/id_mapping.h"
#include "../weights/weighting.h"
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

    int nodeCount() const;
    int edgeCount() const;
    int shortcutCount() const;

    short getNodeLevel(int node) const;

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func) const;

    int getEdgeWeight(EdgeRef edge) const;
    int getTurnCost(EdgeRef from, int via, EdgeRef to) const;

    int getOtherNode(EdgeRef edge, int node) const;
};
