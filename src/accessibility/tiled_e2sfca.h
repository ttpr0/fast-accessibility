#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "../solver/one_to_many/grasp.h"
#include "./distance_decay/linear_decay.h"

std::vector<float> calcGRASP2SFCA(ITiledGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                  int max_range)
{
    auto decay = LinearDecay(max_range);
    return calc2SFCA<GRASP>(g, dem_points, dem_weights, sup_points, sup_weights, decay);
}
