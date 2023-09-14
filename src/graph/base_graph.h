#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "./graph.h"
#include "./graph_storage.h"
#include "./topology_storage.h"

//*******************************************
// classes
//*******************************************

class Graph;
class BaseGraphExplorer;
class EdgeRefIterator;
class BaseGraphIndex;

//*******************************************
// base-graph
//*******************************************

class Graph : public IGraph
{
public:
    GraphStore store;
    TopologyStore topology;
    std::vector<int> edge_weights;

    Graph(GraphStore store, TopologyStore topology, std::vector<int> weights) : store(store), topology(topology), edge_weights(weights) {}

    std::unique_ptr<IGraphExplorer> getGraphExplorer();

    std::unique_ptr<IGraphIndex> getIndex();

    int nodeCount();
    int edgeCount();

    Node getNode(int node);
    Edge getEdge(int edge);

    Coord getNodeGeom(int node);
};

//*******************************************
// base-graph explorer
//******************************************

class BaseGraphExplorer : public IGraphExplorer
{
public:
    Graph* graph;
    TopologyAccessor accessor;
    std::vector<int>& edge_weights;

    BaseGraphExplorer(Graph* graph, TopologyAccessor accessor, std::vector<int>& weights) : graph(graph), accessor(accessor), edge_weights(weights) {}

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func);

    int getEdgeWeight(EdgeRef edge);

    int getTurnCost(EdgeRef from, int via, EdgeRef to);

    int getOtherNode(EdgeRef edge, int node);
};

//*******************************************
// graph index
//*******************************************

class BaseGraphIndex : public IGraphIndex
{
public:
    std::vector<Coord>& node_geoms;

    BaseGraphIndex(std::vector<Coord>& node_geoms) : node_geoms(node_geoms) {}

    int getClosestNode(Coord point);
};
