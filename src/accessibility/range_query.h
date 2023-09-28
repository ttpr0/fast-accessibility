#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/all_dijkstra.h"
#include "../algorithm/range_dijkstra.h"
#include "../algorithm/range_gs_phast.h"
#include "../algorithm/range_phast.h"
#include "../algorithm/range_rphast.h"
#include "../algorithm/rphast_preprocess.h"
#include "../algorithm/util.h"
#include "../graph/graph.h"

std::vector<int> calcDijkstraRangeQuery(IGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    for (int i = 0; i < target_points.size(); i++) {
        target_nodes[i] = index.getClosestNode(target_points[i]);
    }

    auto flags = DistFlagArray(g->nodeCount());

    calcAllDijkstra(g, 100, flags);

    flags.soft_reset();
    // compute dist
    calcRangeDijkstra(g, start_node, flags, max_range);
    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}

std::vector<int> calcPHASTRangeQuery(ICHGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    for (int i = 0; i < target_points.size(); i++) {
        target_nodes[i] = index.getClosestNode(target_points[i]);
    }

    auto flags = DistFlagArray(g->nodeCount());

    // compute dist
    calcRangePHAST5(g, start_node, flags, max_range);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}

std::vector<int> calcRPHASTRangeQuery(ICHGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    std::vector<bool> is_target(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        is_target[i] = false;
    }
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            is_target[node] = true;
        }
    }
    std::queue<int> node_queue;
    for (int i = 0; i < g->nodeCount(); i++) {
        if (is_target[i]) {
            node_queue.push(i);
        }
    }

    auto flags = DistFlagArray(g->nodeCount());

    auto down_edges_subset = preprocessRPHAST(g, std::move(node_queue));

    // compute dist
    calcRangeRPHAST(g, start_node, flags, max_range, down_edges_subset);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}

std::vector<int> calcGSPHASTRangeQuery(CHGraph2* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    for (int i = 0; i < target_points.size(); i++) {
        target_nodes[i] = index.getClosestNode(target_points[i]);
    }

    auto flags = DistFlagArray(g->nodeCount());

    // compute dist
    calcGSPHAST(g, start_node, flags, max_range);

    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        if (target_node == -1) {
            ranges[i] = -9999;
            continue;
        }
        auto target_flag = flags[target_node];
        if (!target_flag.visited) {
            ranges[i] = -9999;
        } else {
            ranges[i] = target_flag.dist;
        }
    }

    return ranges;
}
