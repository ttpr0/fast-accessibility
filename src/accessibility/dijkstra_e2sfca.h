#pragma once

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "../graph/util/map_coords.h"
#include "../solver/one_to_many/dijkstra.h"
#include "./2sfca.h"
#include "./distance_decay/linear_decay.h"

// 2sfca using range-dijkstra
std::vector<float> calcDijkstra2SFCA(IGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                     int max_range)
{
    auto decay = LinearDecay(max_range);
    RangeDijkstra solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    return calc2SFCA<RangeDijkstra>(solver, dem_nodes, dem_weights, sup_nodes, sup_weights, decay);
}
