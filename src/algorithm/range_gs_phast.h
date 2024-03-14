#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/ch_graph.h"
#include "../graph/graph.h"
#include "./util.h"

// RangePHAST with graph-partitioning
void calcGSPHAST(CHGraph2* g, DSnap start, Flags<DistFlag>& flags, int max_range, std::vector<bool>& contains_targets, std::vector<bool>& is_found)
{
    std::priority_queue<pq_item> heap;
    for (int i = 0; i < start.len(); i++) {
        auto s = start[i];
        auto& start_flag = flags[s.node];
        start_flag.dist = s.dist;

        heap.push({s.node, s.dist});
    }

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
        short curr_tile = g->getNodeTile(curr_id);
        is_found[curr_tile] = true;
        g->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&flags, &g, &heap, &max_range, &curr_flag](EdgeRef ref) {
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
    // iterative down-sweep
    const std::vector<Shortcut>& down_edges = g->getDownEdges(Direction::FORWARD);
    auto overlay_dummy = down_edges[0];
    int overlay_start = 1;
    int overlay_end = 1 + overlay_dummy.to;
    for (int i = overlay_start; i < overlay_end; i++) {
        auto edge = down_edges[i];
        auto& curr_flag = flags[edge.from];
        int new_len = curr_flag.dist + edge.weight;
        if (new_len > max_range) {
            continue;
        }
        auto& other_flag = flags[edge.to];
        if (other_flag.dist > new_len) {
            other_flag.dist = new_len;
            other_flag.visited = true;
            short to_tile = edge.payload.get<short>(0);
            is_found[to_tile] = true;
        }
    }
    for (int i = overlay_end; i < down_edges.size(); i++) {
        auto curr_dummy = down_edges[i];
        int curr_tile = curr_dummy.payload.get<short>(0);
        int curr_count = curr_dummy.to;
        if (is_found[curr_tile] && contains_targets[curr_tile]) {
            int tile_start = i + 1;
            int tile_end = i + 1 + curr_count;
            for (int j = tile_start; j < tile_end; j++) {
                auto edge = down_edges[j];
                auto& curr_flag = flags[edge.from];
                int new_len = curr_flag.dist + edge.weight;
                if (new_len > max_range) {
                    continue;
                }
                auto& other_flag = flags[edge.to];
                if (other_flag.dist > new_len) {
                    other_flag.dist = new_len;
                    other_flag.visited = true;
                }
            }
        }
        i += curr_count;
    }
}
