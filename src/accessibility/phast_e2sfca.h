#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "../solver/one_to_many/phast.h"
#include "../solver/one_to_many/rphast.h"
#include "./distance_decay/linear_decay.h"

// 2sfca using range-PHAST
std::vector<float> calcRangePHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                       int max_range)
{
    auto decay = LinearDecay(max_range);
    return calc2SFCA<RangePHAST>(g, dem_points, dem_weights, sup_points, sup_weights, decay);
}

// rphast using queue
std::vector<float> calcRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                   int max_range)
{
    auto decay = LinearDecay(max_range);
    return calc2SFCA<RPHAST>(g, dem_points, dem_weights, sup_points, sup_weights, decay);
}

// range rphast using queue
std::vector<float> calcRangeRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                        int max_range)
{
    auto decay = LinearDecay(max_range);
    return calc2SFCA<RangeRPHAST>(g, dem_points, dem_weights, sup_points, sup_weights, decay);
}

// range rphast using priority queue
std::vector<float> calcRangeRPHAST2SFCA2(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                         int max_range)
{
    auto decay = LinearDecay(max_range);
    return calc2SFCA<RangeRPHAST2>(g, dem_points, dem_weights, sup_points, sup_weights, decay);
}

// range-rphast+gs using queue
std::vector<float> calcGSRPHAST2SFCA(CHGraph2* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                     int max_range)
{
    auto decay = LinearDecay(max_range);
    return calc2SFCA<RangeRPHASTGS>(g, dem_points, dem_weights, sup_points, sup_weights, decay);
}
