#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "../../kd_tree/kd_tree.h"
#include "../graph.h"
#include "../structs/adjacency.h"
#include "../structs/id_mapping.h"
#include "./graph_base.h"

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

KDTree build_kdtree(std::vector<Coord>& node_geoms);
KDTree build_balanced_kdtree(std::vector<Coord>& node_geoms);

class KDTreeIndex : public IGraphIndex
{
public:
    KDTree& tree;

    KDTreeIndex(KDTree& tree) : tree(tree) {}

    int getClosestNode(Coord point);
};

class MappedKDTreeIndex : public IGraphIndex
{
public:
    _IDMapping& id_mapping;
    KDTree& tree;

    MappedKDTreeIndex(KDTree& tree, _IDMapping& id_mapping) : tree(tree), id_mapping(id_mapping) {}

    int getClosestNode(Coord point);
};

// std::unique_ptr<IGraphIndex> build_kdtree_index(std::vector<Coord>& node_geoms);
// std::unique_ptr<IGraphIndex> build_mapped_kdtree_index(std::vector<Coord>& node_geoms, _IDMapping id_mapping);
