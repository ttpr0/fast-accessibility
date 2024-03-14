#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./util.h"

// computes dijkstra forward search until all targets are found
void calcRestrictedDijkstra(IGraph* g, DSnap start, Flags<DistFlag>& flags, std::vector<bool>& targets, int target_count)
{
    std::priority_queue<pq_item> heap;
    for (int i = 0; i < start.len(); i++) {
        auto s = start[i];
        auto& start_flag = flags[s.node];
        start_flag.dist = s.dist;

        heap.push({s.node, s.dist});
    }

    int found_count = 0;
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        auto& curr_flag = flags[curr_id];
        if (curr_flag.visited) {
            continue;
        }
        curr_flag.visited = true;
        if (targets[curr_id]) {
            found_count += 1;
            if (found_count == target_count) {
                break;
            }
        }
        g->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&flags, &g, &heap, &curr_flag](EdgeRef ref) {
            if (ref.isShortcut()) {
                return;
            }
            int other_id = ref.other_id;
            auto& other_flag = flags[other_id];
            if (other_flag.visited) {
                return;
            }
            int new_length = curr_flag.dist + g->getEdgeWeight(ref);
            if (other_flag.dist > new_length) {
                other_flag.dist = new_length;
                heap.push({other_id, new_length});
            }
        });
    }
}

// computes dijkstra forward search until "max_range" is reached or all targets are found
void calcRestrictedRangeDijkstra(IGraph* g, DSnap start, Flags<DistFlag>& flags, int max_range, std::vector<bool>& targets, int target_count)
{
    std::priority_queue<pq_item> heap;
    for (int i = 0; i < start.len(); i++) {
        auto s = start[i];
        auto& start_flag = flags[s.node];
        start_flag.dist = s.dist;

        heap.push({s.node, s.dist});
    }

    int found_count = 0;
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        auto& curr_flag = flags[curr_id];
        if (curr_flag.visited) {
            continue;
        }
        curr_flag.visited = true;
        if (targets[curr_id]) {
            found_count += 1;
            if (found_count == target_count) {
                break;
            }
        }
        g->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&flags, &g, &heap, &max_range, &curr_flag](EdgeRef ref) {
            if (ref.isShortcut()) {
                return;
            }
            int other_id = ref.other_id;
            auto& other_flag = flags[other_id];
            if (other_flag.visited) {
                return;
            }
            int new_length = curr_flag.dist + g->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (other_flag.dist > new_length) {
                other_flag.dist = new_length;
                heap.push({other_id, new_length});
            }
        });
    }
}
