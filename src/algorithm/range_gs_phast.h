#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/ch_graph.h"
#include "../graph/graph.h"
#include "./util.h"

// RangePHAST with graph-partitioning
void calcGSPHAST(CHGraph2* g, int start, Flags<DistFlag>& flags, int max_range, std::vector<bool>& contains_targets, std::vector<bool>& is_found)
{
    auto& start_flag = flags[start];
    start_flag.dist = 0;

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});

    auto& explorer = g->getGraphExplorer();

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
        explorer.forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&flags, &explorer, &heap, &max_range, &curr_flag](EdgeRef ref) {
            int other_id = ref.other_id;
            auto& other_flag = flags[other_id];
            if (other_flag.visited) {
                return;
            }
            int new_length = curr_flag.dist + explorer.getEdgeWeight(ref);
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
    const std::vector<CHEdge4>& down_edges = g->getDownEdges4(Direction::FORWARD);
    CHEdge4 overlay_dummy = down_edges[0];
    int overlay_start = 1;
    int overlay_end = 1 + overlay_dummy.to;
    for (int i = overlay_start; i < overlay_end; i++) {
        CHEdge4 edge = down_edges[i];
        auto& curr_flag = flags[edge.from];
        int new_len = curr_flag.dist + edge.weight;
        if (new_len > max_range) {
            continue;
        }
        auto& other_flag = flags[edge.to];
        if (other_flag.dist > new_len) {
            other_flag.dist = new_len;
            other_flag.visited = true;
            is_found[edge.to_tile] = true;
        }
    }
    for (int i = overlay_end; i < down_edges.size(); i++) {
        CHEdge4 curr_dummy = down_edges[i];
        int curr_tile = curr_dummy.to_tile;
        int curr_count = curr_dummy.to;
        if (is_found[curr_tile] && contains_targets[curr_tile]) {
            int tile_start = i + 1;
            int tile_end = i + 1 + curr_count;
            for (int j = tile_start; j < tile_end; j++) {
                CHEdge4 edge = down_edges[j];
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
