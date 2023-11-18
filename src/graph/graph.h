#pragma once

#include <functional>
#include <memory>
#include <span>
#include <vector>

#include "../util.h"
#include "./structs/geom.h"
#include "./structs/structs.h"

//*******************************************
// graph interfaces
//*******************************************

class IGraphExplorer
{
public:
    virtual ~IGraphExplorer() {}

    virtual void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func) = 0;
    virtual int getEdgeWeight(EdgeRef edge) = 0;
    virtual int getTurnCost(EdgeRef from, int via, EdgeRef to) = 0;
    virtual int getOtherNode(EdgeRef edge, int node) = 0;
};

class IGraphIndex
{
public:
    virtual ~IGraphIndex() {}

    virtual int getClosestNode(Coord point) = 0;
};

class IGraph
{
public:
    virtual ~IGraph() {}

    virtual IGraphExplorer& getGraphExplorer() = 0;
    virtual IGraphIndex& getIndex() = 0;
    virtual int nodeCount() = 0;
    virtual int edgeCount() = 0;
    virtual Node getNode(int node) = 0;
    virtual Edge getEdge(int edge) = 0;
    virtual Coord getNodeGeom(int node) = 0;
};

class ICHGraph : public IGraph
{
public:
    virtual ~ICHGraph() {}

    virtual short getNodeLevel(int node) = 0;
    virtual int shortcutCount() = 0;
    virtual Shortcut getShortcut(int shortcut) = 0;
    virtual const std::vector<CHEdge>& getDownEdges(Direction dir) = 0;
};

class ITiledGraph : public IGraph
{
public:
    virtual ~ITiledGraph() {}

    virtual short getNodeTile(int node) = 0;
    virtual short tileCount() = 0;
    virtual int shortcutCount() = 0;
    virtual Shortcut getShortcut(int shortcut) = 0;
    virtual const std::span<Shortcut> getIndexEdges(short tile, Direction dir) = 0;
};