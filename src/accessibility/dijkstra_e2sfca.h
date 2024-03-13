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
#include "./2sfca.h"
#include "./distance_decay/linear_decay.h"

// 2sfca using range-dijkstra
Vector<float> calcDijkstra2SFCA(IGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                int max_range)
{
    auto decay = LinearDecay(max_range);
    RangeDijkstra solver = {g};
    // auto dem_nodes = map_to_closest(dem_points, *g);
    // auto sup_nodes = map_to_closest(sup_points, *g);
    std::vector<int> dem_nodes;
    std::vector<int> sup_nodes;
    auto dem_n = VectorView<int>(dem_nodes.data(), dem_nodes.size(), {1, 1});
    auto dem_w = VectorView<int>(dem_weights.data(), dem_weights.size(), {1, 1});
    auto sup_n = VectorView<int>(sup_nodes.data(), sup_nodes.size(), {1, 1});
    auto sup_w = VectorView<int>(sup_weights.data(), sup_weights.size(), {1, 1});
    return calc2SFCA<RangeDijkstra>(solver, dem_n, dem_w, sup_n, sup_w, decay);
}
