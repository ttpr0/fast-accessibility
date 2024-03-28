#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <span>
#include <vector>

#include "../util.h"
#include "../util/function_ref.h"
#include "./structs/geom.h"
#include "./structs/structs.h"

//*******************************************
// graph concepts
//*******************************************

template <typename T>
concept any_graph = requires(T t, int node, int edge, Direction dir, Adjacency adj, function_ref<void(EdgeRef)> func, EdgeRef ref) {
    {
        t.nodeCount()
    } -> std::same_as<int>;
    {
        t.edgeCount()
    } -> std::same_as<int>;
    {
        t.getNode(node)
    } -> std::same_as<Node>;
    {
        t.getEdge(edge)
    } -> std::same_as<Edge>;
    {
        t.getNodeGeom(node)
    } -> std::same_as<Coord>;
    {
        t.forAdjacentEdges(node, dir, adj, func)
    } -> std::same_as<void>;
    {
        t.getEdgeWeight(ref)
    } -> std::same_as<int>;
    {
        t.getOtherNode(ref, node)
    } -> std::same_as<int>;
};

template <typename T>
concept any_tc_graph = any_graph<T> && requires(T t, EdgeRef from, int via, EdgeRef to) {
    {
        t.getTurnCost(from, via, to)
    } -> std::same_as<int>;
};

template <typename T>
concept any_ch_graph = any_graph<T> && requires(T t, int node, int shc, Direction dir) {
    {
        t.getNodeLevel(node)
    } -> std::same_as<short>;
    {
        t.shortcutCount()
    } -> std::same_as<int>;
    {
        t.getShortcut(shc)
    } -> std::same_as<Shortcut>;
};

template <typename T>
concept any_phast_graph = any_ch_graph<T> && requires(T t, int shc, Direction dir) {
    {
        t.getDownEdges(dir)
    } -> std::same_as<const std::vector<Shortcut>&>;
};

template <typename T>
concept any_tiled_graph = any_graph<T> && requires(T t, int node, int shc) {
    {
        t.getNodeTile(node)
    } -> std::same_as<short>;
    {
        t.tileCount()
    } -> std::same_as<short>;
    {
        t.shortcutCount()
    } -> std::same_as<int>;
    {
        t.getShortcut(shc)
    } -> std::same_as<Shortcut>;
};

template <typename T>
concept any_grasp_graph = any_tiled_graph<T> && requires(T t, short tile, Direction dir) {
    {
        t.getIndexEdges(tile, dir)
    } -> std::same_as<const std::span<Shortcut>>;
};

//*******************************************
// graph interfaces
//*******************************************

class IGraph
{
public:
    virtual ~IGraph() {}

    virtual int nodeCount() = 0;
    virtual int edgeCount() = 0;
    virtual Node getNode(int node) = 0;
    virtual Edge getEdge(int edge) = 0;
    virtual Coord getNodeGeom(int node) = 0;
    virtual void forAdjacentEdges(int node, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func) = 0;
    virtual int getEdgeWeight(EdgeRef edge) = 0;
    virtual int getTurnCost(EdgeRef from, int via, EdgeRef to) = 0;
    virtual int getOtherNode(EdgeRef edge, int node) = 0;
};

class ICHGraph : public IGraph
{
public:
    virtual ~ICHGraph() {}

    virtual short getNodeLevel(int node) = 0;
    virtual int shortcutCount() = 0;
    virtual Shortcut getShortcut(int shortcut) = 0;
    virtual const std::vector<Shortcut>& getDownEdges(Direction dir) = 0;
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
