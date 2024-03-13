#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "../../util/kd_tree/kd_tree.h"
#include "../../util/matrix.h"
#include "./graph_base.h"
#include "./id_mapping.h"

//*******************************************
// graph index
//*******************************************

class IGraphIndex
{
public:
    virtual ~IGraphIndex() {}

    virtual int getClosestNode(Coord point) = 0;
    virtual int getClosestNode(float lon, float lat) = 0;
    virtual int getClosestNode(Coord point, const _IDMapping& id_mapping) = 0;
    virtual int getClosestNode(float lon, float lat, const _IDMapping& id_mapping) = 0;

    virtual std::vector<int> mapToClosest(const std::vector<Coord>& coords) = 0;
    virtual Vector<int> mapToClosest(VectorView<float> lons, VectorView<float> lats) = 0;
    virtual std::vector<int> mapToClosest(const std::vector<Coord>& coords, const _IDMapping& id_mapping) = 0;
    virtual Vector<int> mapToClosest(VectorView<float> lons, VectorView<float> lats, const _IDMapping& id_mapping) = 0;
};

class KDTreeIndex : public IGraphIndex
{
public:
    KDTree tree;

    KDTreeIndex(KDTree tree) : tree(std::move(tree)) {}

    int getClosestNode(Coord point);
    int getClosestNode(float lon, float lat);
    int getClosestNode(Coord point, const _IDMapping& id_mapping);
    int getClosestNode(float lon, float lat, const _IDMapping& id_mapping);

    std::vector<int> mapToClosest(const std::vector<Coord>& coords);
    Vector<int> mapToClosest(VectorView<float> lons, VectorView<float> lats);
    std::vector<int> mapToClosest(const std::vector<Coord>& coords, const _IDMapping& id_mapping);
    Vector<int> mapToClosest(VectorView<float> lons, VectorView<float> lats, const _IDMapping& id_mapping);
};

std::shared_ptr<IGraphIndex> build_kdtree_index(GraphBase& base);
std::shared_ptr<IGraphIndex> build_balanced_kdtree_index(GraphBase& base);
