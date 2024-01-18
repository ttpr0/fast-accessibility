#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "./base_graph.h"
#include "./comps/graph_index.h"
#include "./comps/id_mapping.h"
#include "./graph.h"
#include "./speed_ups/ch_data.h"
#include "./speed_ups/ch_index.h"
#include "./speed_ups/partition.h"
#include "./structs/adjacency.h"

//*******************************************
// ch-graph
//*******************************************

class CHGraph : public ICHGraph
{
public:
    std::shared_ptr<GraphBase> base;
    std::shared_ptr<Weighting> weights;
    std::shared_ptr<IGraphIndex> index;

    // additional components
    std::shared_ptr<_IDMapping> id_mapping;
    std::shared_ptr<CHData> ch;
    // ch-index
    std::shared_ptr<_CHIndex> ch_index;

    CHGraph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index, std::shared_ptr<_IDMapping> id_mapping,
            std::shared_ptr<CHData> ch, std::shared_ptr<_CHIndex> ch_index);

    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);
    Coord getNodeGeom(int node);
    int getClosestNode(Coord point);

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);

    short getNodeLevel(int node);
    int shortcutCount();
    Shortcut getShortcut(int shortcut);
    const std::vector<Shortcut>& getDownEdges(Direction dir);
};

class CHGraph2 : public IGraph
{
public:
    std::shared_ptr<GraphBase> base;
    std::shared_ptr<Weighting> weights;
    std::shared_ptr<IGraphIndex> index;
    std::shared_ptr<Partition> partition;

    // additional components
    std::shared_ptr<_IDMapping> id_mapping;
    std::shared_ptr<CHData> ch;
    // ch-index
    std::shared_ptr<_CHIndex> ch_index;

    CHGraph2(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index, std::shared_ptr<Partition> partition,
             std::shared_ptr<_IDMapping> id_mapping, std::shared_ptr<CHData> ch, std::shared_ptr<_CHIndex> ch_index);

    int nodeCount();
    int edgeCount();
    Node getNode(int node);
    Edge getEdge(int edge);
    Coord getNodeGeom(int node);
    int getClosestNode(Coord point);

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);
    int getEdgeWeight(EdgeRef edge);
    int getTurnCost(EdgeRef from, int via, EdgeRef to);
    int getOtherNode(EdgeRef edge, int node);

    short getNodeLevel(int node);
    int shortcutCount();
    Shortcut getShortcut(int shortcut);
    const std::vector<Shortcut>& getDownEdges(Direction dir);
    short getNodeTile(int node);
    int tileCount();
};

//*******************************************
// build ch-graph
//*******************************************

CHGraph build_ch_graph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index, std::shared_ptr<_IDMapping> id_mapping,
                       std::shared_ptr<CHData> ch, std::shared_ptr<_CHIndex> ch_index);
CHGraph2 build_ch_graph_2(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<IGraphIndex> index, std::shared_ptr<Partition> partition,
                          std::shared_ptr<_IDMapping> id_mapping, std::shared_ptr<CHData> ch, std::shared_ptr<_CHIndex> ch_index);
