#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "../graph/util/map_coords.h"
#include "../solver/one_to_many/phast.h"
#include "../solver/one_to_many/rphast.h"
#include "./2sfca.h"
#include "./distance_decay/linear_decay.h"

// 2sfca using range-PHAST
std::vector<float> calcRangePHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                       int max_range)
{
    auto decay = LinearDecay(max_range);
    RangePHAST solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    return calc2SFCA<RangePHAST>(solver, dem_nodes, dem_weights, sup_nodes, sup_weights, decay);
}

// rphast using queue
std::vector<float> calcRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                   int max_range)
{
    auto decay = LinearDecay(max_range);
    RPHAST solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    return calc2SFCA<RPHAST>(solver, dem_nodes, dem_weights, sup_nodes, sup_weights, decay);
}

// range rphast using queue
std::vector<float> calcRangeRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                        int max_range)
{
    auto decay = LinearDecay(max_range);
    RangeRPHAST solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    return calc2SFCA<RangeRPHAST>(solver, dem_nodes, dem_weights, sup_nodes, sup_weights, decay);
}

// range rphast using priority queue
std::vector<float> calcRangeRPHAST2SFCA2(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                         int max_range)
{
    auto decay = LinearDecay(max_range);
    RangeRPHAST2 solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    return calc2SFCA<RangeRPHAST2>(solver, dem_nodes, dem_weights, sup_nodes, sup_weights, decay);
}

// range-rphast+gs using queue
std::vector<float> calcGSRPHAST2SFCA(CHGraph2* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                     int max_range)
{
    auto decay = LinearDecay(max_range);
    RangeRPHASTGS solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    return calc2SFCA<RangeRPHASTGS>(solver, dem_nodes, dem_weights, sup_nodes, sup_weights, decay);
}
