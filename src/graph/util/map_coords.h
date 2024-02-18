#pragma once

#include <vector>

#include "../../util/matrix.h"
#include "../graph.h"
#include "../structs/geom.h"

static std::vector<int> map_to_closest(const std::vector<Coord>& coords, IGraph& graph)
{
    std::vector<int> closest(coords.size());
    for (int i = 0; i < coords.size(); i++) {
        closest[i] = graph.getClosestNode(coords[i]);
    }
    return closest;
}

static Vector<int> map_to_closest(VectorView<float> lons, VectorView<float> lats, IGraph& graph)
{
    Vector<int> closest(lons.rows());
    for (int i = 0; i < lons.rows(); i++) {
        closest(i) = graph.getClosestNode({lons(i), lats(i)});
    }
    return closest;
}

static int map_to_closest(Coord coord, IGraph& graph)
{
    return graph.getClosestNode(coord);
}

static int map_to_closest(float lon, float lat, IGraph& graph)
{
    return graph.getClosestNode({lon, lat});
}
