#pragma once

#include <functional>
#include <vector>

#include "../../util/function_ref.h"
#include "../base/graph_base.h"
#include "../base/id_mapping.h"
#include "../speed_ups/ch_data.h"
#include "../speed_ups/partition.h"
#include "../structs/structs.h"
#include "../weights/weighting.h"

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
    Node getNode(int node) const;
    Edge getEdge(int edge) const;
    Coord getNodeGeom(int node) const;

    short getNodeLevel(int node) const;
    int shortcutCount() const;
    Shortcut getShortcut(int shortcut) const;

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func) const;
    int getEdgeWeight(EdgeRef edge) const;
    int getOtherNode(EdgeRef edge, int node) const;
};
