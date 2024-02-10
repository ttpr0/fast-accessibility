#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "../../util/kd_tree/kd_tree.h"
#include "./graph_base.h"

//*******************************************
// graph index
//*******************************************

class IGraphIndex
{
public:
    virtual ~IGraphIndex() {}

    virtual std::tuple<int, bool> getClosestNode(Coord point) = 0;
};

class BaseGraphIndex : public IGraphIndex
{
public:
    std::vector<Node>& nodes;

    BaseGraphIndex(std::vector<Node>& nodes) : nodes(nodes) {}

    std::tuple<int, bool> getClosestNode(Coord point);
};

class KDTreeIndex : public IGraphIndex
{
public:
    KDTree tree;

    KDTreeIndex(KDTree tree) : tree(std::move(tree)) {}

    std::tuple<int, bool> getClosestNode(Coord point);
};

std::shared_ptr<IGraphIndex> build_base_index(GraphBase& base);
std::shared_ptr<IGraphIndex> build_kdtree_index(GraphBase& base);
std::shared_ptr<IGraphIndex> build_balanced_kdtree_index(GraphBase& base);
