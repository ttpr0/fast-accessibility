#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../../../graph/graph.h"
#include "../../../util/flags.h"
#include "../../../util/pq_item.h"
#include "../../../util/snap.h"

// simple RangePHAST
template <typename TGraph>
    requires any_phast_graph<TGraph>
void calcRangePHAST(TGraph& g, DSnap start, Flags<DistFlag>& flags, int max_range)
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
        g.forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&flags, &g, &heap, &curr_flag, &max_range](EdgeRef ref) {
            int other_id = ref.other_id;
            auto& other_flag = flags[other_id];
            if (other_flag.visited) {
                return;
            }
            int new_length = curr_flag.dist + g.getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (other_flag.dist > new_length) {
                other_flag.dist = new_length;
                heap.push({other_id, new_length});
            }
        });
    }

    const std::vector<Shortcut>& down_edges = g.getDownEdges(Direction::FORWARD);
    int length = down_edges.size();
    for (int i = 0; i < length; i++) {
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
        }
    }
}
