#pragma once

#include <vector>

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

static int map_to_closest(Coord coord, IGraph& graph)
{
    return graph.getClosestNode(coord);
}
