#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "./base/graph_base.h"
#include "./base/graph_index.h"
#include "./base/id_mapping.h"
#include "./base_graph.h"
#include "./graph.h"
#include "./speed_ups/cell_index.h"
#include "./speed_ups/ch_data.h"
#include "./speed_ups/partition.h"
#include "./speed_ups/tiled_data.h"
#include "./structs/adjacency.h"
#include "../util/function_ref.h"

//*******************************************
// tiled-graph
//*******************************************

class TiledGraph : public ITiledGraph
{
public:
    std::shared_ptr<GraphBase> base;
    std::shared_ptr<Weighting> weights;
    std::shared_ptr<IGraphIndex> index;
    std::shared_ptr<Partition> partition;

    // additional components
    std::shared_ptr<_IDMapping> id_mapping;
    std::shared_ptr<TiledData> tiled;
    // cell-index
    std::shared_ptr<_CellIndex> cell_index;

    TiledGraph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index, std::shared_ptr<Partition> partition,
               std::shared_ptr<_IDMapping> id_mapping, std::shared_ptr<TiledData> tiled, std::shared_ptr<_CellIndex> cell_index);

    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);
    Coord getNodeGeom(int node);
    int getClosestNode(Coord point);

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);

    short getNodeTile(int node);
    short tileCount();
    int shortcutCount();
    Shortcut getShortcut(int shortcut);
    const std::span<Shortcut> getIndexEdges(short tile, Direction dir);
};

//*******************************************
// build tiled-graph
//*******************************************

TiledGraph build_tiled_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index, std::shared_ptr<Partition> partition,
                             std::shared_ptr<_IDMapping> id_mapping, std::shared_ptr<TiledData> tiled, std::shared_ptr<_CellIndex> cell_index);
