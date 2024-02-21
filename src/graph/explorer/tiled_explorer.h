#pragma once

#include <functional>
#include <vector>

#include "../../util/function_ref.h"
#include "../base/graph_base.h"
#include "../base/id_mapping.h"
#include "../speed_ups/partition.h"
#include "../speed_ups/tiled_data.h"
#include "../structs/structs.h"
#include "../weights/weighting.h"

//*******************************************
// tiled-graph explorer
//*******************************************

class TiledGraphExplorer
{
public:
    const GraphBase& base;
    const Weighting& weights;
    const Partition& partition;
    const TiledData& tiled;
    const _IDMapping& id_mapping;

    TiledGraphExplorer(const GraphBase& base, const Weighting& weights, const Partition& partition, const TiledData& tiled, const _IDMapping& id_mapping)
        : base(base), weights(weights), partition(partition), tiled(tiled), id_mapping(id_mapping)
    {}

    int nodeCount() const;
    int edgeCount() const;
    int shortcutCount() const;

    short getNodeTile(int node) const;

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func) const;

    int getEdgeWeight(EdgeRef edge) const;
    int getTurnCost(EdgeRef from, int via, EdgeRef to) const;

    int getOtherNode(EdgeRef edge, int node) const;
};
