#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/range_gs_rphast.h"
#include "../algorithm/range_phast.h"
#include "../algorithm/range_rphast.h"
#include "../algorithm/rphast.h"
#include "../algorithm/rphast_preprocess.h"
#include "../algorithm/util.h"
#include "../graph/graph.h"
#include "../util.h"

// 2sfca using range-PHAST
std::vector<float> calcRangePHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                       int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
    }

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = DistFlagArray(g->nodeCount());
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcRangePHAST(g, s_id, flags, max_range);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist == 1000000000) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist == 1000000000) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            m.lock();
            access[i] += R * distance_decay;
            m.unlock();
        }
    }
    return access;
}

// rphast using queue
std::vector<float> calcRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                   int max_range)
{
    // get closest node for every demand point
    std::queue<int> node_queue;
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push(id);
        }
    }

    // select graph subset
    auto down_edges_subset = preprocessRPHAST(g, std::move(node_queue));

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = DistFlagArray(g->nodeCount());
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcRPHAST(g, s_id, flags, down_edges_subset);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            access[i] += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}

// range rphast using queue
std::vector<float> calcRangeRPHAST2SFCA(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                        int max_range)
{
    // get closest node for every demand point
    std::queue<int> node_queue;
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push(id);
        }
    }

    // select graph subset
    auto down_edges_subset = preprocessRPHAST(g, std::move(node_queue));

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = DistFlagArray(g->nodeCount());
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcRangeRPHAST(g, s_id, flags, max_range, down_edges_subset);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            m.lock();
            access[i] += R * distance_decay;
            m.unlock();
        }
    }
    return access;
}

// range rphast using priority queue
std::vector<float> calcRangeRPHAST2SFCA2(ICHGraph* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                         int max_range)
{
    // get closest node for every demand point
    std::priority_queue<pq_item> node_queue;
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push({id, 0});
        }
    }

    // select graph subset
    auto down_edges_subset = preprocessRangeRPHAST(g, std::move(node_queue), max_range);

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = DistFlagArray(g->nodeCount());
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcRangeRPHAST(g, s_id, flags, max_range, down_edges_subset);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist > max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist > max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            access[i] += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}

// range-rphast+gs using queue
std::vector<float> calcGSRPHAST2SFCA(CHGraph2* g, std::vector<Coord>& dem_points, std::vector<int>& dem_weights, std::vector<Coord>& sup_points, std::vector<int>& sup_weights,
                                     int max_range)
{
    // get closest node for every demand point
    std::queue<int> node_queue;
    IGraphIndex& index = g->getIndex();
    std::vector<int> dem_nodes(dem_points.size());
    for (int i = 0; i < dem_points.size(); i++) {
        auto loc = dem_points[i];
        auto id = index.getClosestNode(loc);
        dem_nodes[i] = id;
        if (id >= 0) {
            node_queue.push(id);
        }
    }

    // select graph subset
    auto down_edges_subset = preprocessGSRPHAST(g, std::move(node_queue));

    // create array containing accessibility results
    std::vector<float> access(dem_points.size());
    for (int i = 0; i < access.size(); i++) {
        access[i] = 0;
    }

    auto flags = DistFlagArray(g->nodeCount());
    std::mutex m;
#pragma omp parallel for firstprivate(flags)
    for (int i = 0; i < sup_points.size(); i++) {
        // get supply information
        int s_id = index.getClosestNode(sup_points[i]);
        if (s_id < 0) {
            continue;
        }
        int s_weight = sup_weights[i];

        // compute distances
        flags.soft_reset();
        calcGSRPHAST(g, s_id, flags, max_range, down_edges_subset);

        // compute R-value for facility
        float demand_sum = 0.0;
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            demand_sum += dem_weights[i] * distance_decay;
        }
        float R = s_weight / demand_sum;
        // add new access to reachable demand points
        m.lock();
        for (int i = 0; i < dem_nodes.size(); i++) {
            int d_node = dem_nodes[i];
            if (d_node == -1) {
                continue;
            }
            auto d_flag = flags[d_node];
            int d_dist = d_flag.dist;
            if (d_dist >= max_range) {
                continue;
            }
            float distance_decay = 1 - d_dist / (float)max_range;
            access[i] += R * distance_decay;
        }
        m.unlock();
    }
    return access;
}