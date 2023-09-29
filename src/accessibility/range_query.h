#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/all_dijkstra.h"
#include "../algorithm/grasp.h"
#include "../algorithm/range_dijkstra.h"
#include "../algorithm/range_gs_phast.h"
#include "../algorithm/range_gs_rphast.h"
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

std::vector<int> calcRangePHASTRangeQuery(ICHGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
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
    std::queue<int> node_queue;
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            node_queue.push(node);
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

std::vector<int> calcRangeRPHASTRangeQuery(ICHGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    std::priority_queue<pq_item> node_queue;
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            node_queue.push({node, 0});
        }
    }

    auto flags = DistFlagArray(g->nodeCount());

    auto down_edges_subset = preprocessRangeRPHAST(g, std::move(node_queue), max_range);

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

std::vector<int> calcGSRPHASTRangeQuery(CHGraph2* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    std::queue<int> node_queue;
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            node_queue.push(node);
        }
    }

    auto flags = DistFlagArray(g->nodeCount());

    std::vector<CHEdge4> subset = preprocessGSRPHAST(g, std::move(node_queue));

    // compute dist
    calcGSRPHAST(g, start_node, flags, max_range, subset);

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

std::vector<int> calcGRASPRangeQuery(ITiledGraph* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    int tile_count = g->tileCount();
    std::vector<bool> found_tiles(tile_count);
    std::vector<bool> active_tiles(tile_count);
    for (int i = 0; i < tile_count; i++) {
        found_tiles[i] = false;
        active_tiles[i] = false;
    }
    for (int i = 0; i < target_points.size(); i++) {
        int node = index.getClosestNode(target_points[i]);
        target_nodes[i] = node;
        if (node >= 0) {
            short tile = g->getNodeTile(node);
            active_tiles[tile] = true;
        }
    }

    auto flags = DistFlagArray(g->nodeCount());

    // compute dist
    calcGRASP(g, start_node, flags, max_range, active_tiles, found_tiles);

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