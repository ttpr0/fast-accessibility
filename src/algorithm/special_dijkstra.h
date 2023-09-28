#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./util.h"

// computes ball around start until "count" nodes are visited
void calcCountDijkstra(IGraph* g, int start, DistFlagArray& flags, int count)
{
    flags.set_start(start);

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
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
        found_count += 1;
        if (found_count == count) {
            break;
        }
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&flags, &explorer, &heap, &curr_flag](EdgeRef ref) {
            if (ref.isShortcut()) {
                return;
            }
            int other_id = ref.other_id;
            auto& other_flag = flags[other_id];
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
}
