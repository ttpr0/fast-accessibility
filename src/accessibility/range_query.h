#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../algorithm/pq_item.h"
#include "../algorithm/range_dijkstra.h"
#include "../algorithm/range_phast.h"
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

    // compute dist
    auto dist = calcRangeDijkstra(g, start_node, max_range);
    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        int range = dist[target_node];
        if (range == 1000000000) {
            ranges[i] = -9999;
        } else {
            ranges[i] = range;
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

    // compute dist
    auto dist = calcRangePHAST(g, start_node, max_range);
    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        int range = dist[target_node];
        if (range == 1000000000) {
            ranges[i] = -9999;
        } else {
            ranges[i] = range;
        }
    }

    return ranges;
}

std::vector<int> calcPHASTRangeQuery2(CHGraph2* g, Coord start_point, std::vector<Coord>& target_points, int max_range)
{
    // get closest node for every demand point
    IGraphIndex& index = g->getIndex();
    int start_node = index.getClosestNode(start_point);
    std::vector<int> target_nodes(target_points.size());
    for (int i = 0; i < target_points.size(); i++) {
        target_nodes[i] = index.getClosestNode(target_points[i]);
    }

    // compute dist
    auto dist = calcRangePHAST5(g, start_node, max_range);
    // create array containing accessibility results
    std::vector<int> ranges(target_nodes.size());
    for (int i = 0; i < ranges.size(); i++) {
        int target_node = target_nodes[i];
        int range = dist[target_node];
        if (range == 1000000000) {
            ranges[i] = -9999;
        } else {
            ranges[i] = range;
        }
    }

    return ranges;
}
