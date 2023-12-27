#pragma once

#include <functional>
#include <vector>

#include "../comps/graph_base.h"
#include "../comps/id_mapping.h"
#include "../comps/weighting.h"
#include "../speed_ups/partition.h"
#include "../speed_ups/tiled_data.h"
#include "../structs/structs.h"

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

    int nodeCount();
    short getNodeTile(int node);

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);
};
