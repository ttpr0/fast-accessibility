#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "./base_graph.h"
#include "./comps/cell_index.h"
#include "./comps/ch_data.h"
#include "./comps/graph_base.h"
#include "./comps/partition.h"
#include "./comps/tiled_data.h"
#include "./graph.h"
#include "./structs/adjacency.h"

//*******************************************
// tiled-graph
//*******************************************

class TiledGraphExplorer;

class TiledGraph : public ITiledGraph
{
public:
    std::shared_ptr<GraphBase> base;
    std::shared_ptr<Weighting> weights;
    std::unique_ptr<IGraphExplorer> explorer;
    std::unique_ptr<IGraphIndex> index;
    std::shared_ptr<Partition> partition;

    // additional components
    std::shared_ptr<TiledData> tiled;
    // cell-index
    std::shared_ptr<_CellIndex> cell_index;

    TiledGraph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::unique_ptr<IGraphIndex> index, std::shared_ptr<Partition> partition,
               std::shared_ptr<TiledData> tiled, std::shared_ptr<_CellIndex> cell_index);

    IGraphExplorer& getGraphExplorer();
    IGraphIndex& getIndex();
    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);
    Coord getNodeGeom(int node);

    short getNodeTile(int node);
    short tileCount();
    int shortcutCount();
    Shortcut getShortcut(int shortcut);
    const std::span<Shortcut> getIndexEdges(short tile, Direction dir);
};

//*******************************************
// tiled-graph explorer
//*******************************************

class TiledGraphExplorer : public IGraphExplorer
{
public:
    GraphBase& base;
    Weighting& weights;
    TiledData& tiled;

    TiledGraphExplorer(GraphBase& base, Weighting& weights, TiledData& tiled) : base(base), weights(weights), tiled(tiled) {}

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);
};

//*******************************************
// build tiled-graph
//*******************************************

TiledGraph build_tiled_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<Partition> partition, std::shared_ptr<TiledData> tiled,
                             std::shared_ptr<_CellIndex> index);
