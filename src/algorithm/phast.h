#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./util.h"

// simple phast
void calcPHAST(ICHGraph* g, int start, DistFlagArray& flags_)
{
    auto& flags = flags_.get_flags();
    short counter = flags_.get_counter();

    flags[start] = {0, false, counter};

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
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
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&flags, &counter, &explorer, &heap, &curr_flag](EdgeRef ref) {
            int other_id = ref.other_id;
            auto& other_flag = flags[other_id];
            if (other_flag._flag_counter != counter) {
                other_flag = {1000000000, false, counter};
            }
            if (other_flag.visited) {
                return;
            }
            int new_length = curr_flag.dist + explorer->getEdgeWeight(ref);
            if (other_flag.dist > new_length) {
                other_flag.dist = new_length;
                heap.push({other_id, new_length});
            }
        });
    }

    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    int length = down_edges.size();
    for (int i = 0; i < length; i++) {
        auto edge = down_edges[i];
        auto curr_flag = flags[edge.from];
        if (curr_flag._flag_counter != counter) {
            continue;
        }
        int new_len = curr_flag.dist + edge.weight;
        auto& other_flag = flags[edge.to];
        if (other_flag._flag_counter != counter) {
            other_flag = {1000000000, false, counter};
        }
        if (other_flag.dist > new_len) {
            other_flag.dist = new_len;
        }
    }
}
