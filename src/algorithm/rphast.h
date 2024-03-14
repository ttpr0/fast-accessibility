#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./util.h"

// simple RPHAST
void calcRPHAST(ICHGraph* g, DSnap start, Flags<DistFlag>& flags, std::vector<Shortcut>& down_edges_subset)
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
        g->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&flags, &g, &heap, &curr_flag](EdgeRef ref) {
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

    int length = down_edges_subset.size();
    for (int i = 0; i < length; i++) {
        auto edge = down_edges_subset[i];
        auto& curr_flag = flags[edge.from];
        int new_len = curr_flag.dist + edge.weight;
        auto& other_flag = flags[edge.to];
        if (other_flag.dist > new_len) {
            other_flag.dist = new_len;
        }
    }
}
