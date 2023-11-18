#pragma once

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "../solver/one_to_many/dijkstra.h"
#include "./distance_decay/linear_decay.h"

// 2sfca using range-dijkstra
std::vector<float> calcDijkstra2SFCA(IGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                     int max_range)
{
    auto decay = LinearDecay(max_range);
    return calc2SFCA<RangeDijkstra>(g, dem_points, dem_weights, sup_points, sup_weights, decay);
}
