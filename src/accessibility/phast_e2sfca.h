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
Vector<float> calcRangePHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                       int max_range)
{
    auto decay = LinearDecay(max_range);
    RangePHAST solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    auto dem_n = VectorView<int>(dem_nodes.data(), dem_nodes.size(), {1, 1});
    auto dem_w = VectorView<int>(dem_weights.data(), dem_weights.size(), {1, 1});
    auto sup_n = VectorView<int>(sup_nodes.data(), sup_nodes.size(), {1, 1});
    auto sup_w = VectorView<int>(sup_weights.data(), sup_weights.size(), {1, 1});
    return calc2SFCA<RangePHAST>(solver, dem_n, dem_w, sup_n, sup_w, decay);
}

// rphast using queue
Vector<float> calcRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                   int max_range)
{
    auto decay = LinearDecay(max_range);
    RPHAST solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    auto dem_n = VectorView<int>(dem_nodes.data(), dem_nodes.size(), {1, 1});
    auto dem_w = VectorView<int>(dem_weights.data(), dem_weights.size(), {1, 1});
    auto sup_n = VectorView<int>(sup_nodes.data(), sup_nodes.size(), {1, 1});
    auto sup_w = VectorView<int>(sup_weights.data(), sup_weights.size(), {1, 1});
    return calc2SFCA<RPHAST>(solver, dem_n, dem_w, sup_n, sup_w, decay);
}

// range rphast using queue
Vector<float> calcRangeRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                        int max_range)
{
    auto decay = LinearDecay(max_range);
    RangeRPHAST solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    auto dem_n = VectorView<int>(dem_nodes.data(), dem_nodes.size(), {1, 1});
    auto dem_w = VectorView<int>(dem_weights.data(), dem_weights.size(), {1, 1});
    auto sup_n = VectorView<int>(sup_nodes.data(), sup_nodes.size(), {1, 1});
    auto sup_w = VectorView<int>(sup_weights.data(), sup_weights.size(), {1, 1});
    return calc2SFCA<RangeRPHAST>(solver, dem_n, dem_w, sup_n, sup_w, decay);
}

// range rphast using priority queue
Vector<float> calcRangeRPHAST2SFCA2(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                         int max_range)
{
    auto decay = LinearDecay(max_range);
    RangeRPHAST2 solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    auto dem_n = VectorView<int>(dem_nodes.data(), dem_nodes.size(), {1, 1});
    auto dem_w = VectorView<int>(dem_weights.data(), dem_weights.size(), {1, 1});
    auto sup_n = VectorView<int>(sup_nodes.data(), sup_nodes.size(), {1, 1});
    auto sup_w = VectorView<int>(sup_weights.data(), sup_weights.size(), {1, 1});
    return calc2SFCA<RangeRPHAST2>(solver, dem_n, dem_w, sup_n, sup_w, decay);
}

// range-rphast+gs using queue
Vector<float> calcGSRPHAST2SFCA(CHGraph2* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                     int max_range)
{
    auto decay = LinearDecay(max_range);
    RangeRPHASTGS solver = {g};
    auto dem_nodes = map_to_closest(dem_points, *g);
    auto sup_nodes = map_to_closest(sup_points, *g);
    auto dem_n = VectorView<int>(dem_nodes.data(), dem_nodes.size(), {1, 1});
    auto dem_w = VectorView<int>(dem_weights.data(), dem_weights.size(), {1, 1});
    auto sup_n = VectorView<int>(sup_nodes.data(), sup_nodes.size(), {1, 1});
    auto sup_w = VectorView<int>(sup_weights.data(), sup_weights.size(), {1, 1});
    return calc2SFCA<RangeRPHASTGS>(solver, dem_n, dem_w, sup_n, sup_w, decay);
}
