#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./util.h"

// reGRASP and isoGRASP combination
void calcGRASP(ITiledGraph* g, int start, DistFlagArray& flags_, int max_range, std::vector<bool>& contains_targets,
               std::vector<bool>& is_found)
{
    auto& flags = flags_.get_flags();
    short counter = flags_.get_counter();

    flags[start] = {0, false, counter};

    short start_tile = g->getNodeTile(start);
    is_found[start_tile] = true;

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});

    // routing loop
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
        auto handler = [&flags, &counter, &explorer, &heap, &max_range, &curr_flag](EdgeRef ref) {
            int other_id = ref.other_id;
            auto& other_flag = flags[other_id];
            if (other_flag._flag_counter != counter) {
                other_flag = {1000000000, false, counter};
            }
            if (other_flag.visited) {
                return;
            }
            int new_length = curr_flag.dist + explorer->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (other_flag.dist > new_length) {
                other_flag.dist = new_length;
                heap.push({other_id, new_length});
            }
        };
        if (curr_tile == start_tile) {
            explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, handler);
        } else {
            is_found[curr_tile] = true;
            explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_SKIP, handler);
        }
    }
    for (int i = 0; i < contains_targets.size(); i++) {
        if (!contains_targets[i] || !is_found[i]) {
            continue;
        }
        auto& down_edges = g->getIndexEdges(i, Direction::FORWARD);
        for (int j = 0; j < down_edges.size(); j++) {
            auto edge = down_edges[j];
            auto curr_flag = flags[edge.from];
            if (curr_flag._flag_counter != counter) {
                continue;
            }
            int new_len = curr_flag.dist + edge.weight;
            if (new_len > max_range) {
                continue;
            }
            auto& other_flag = flags[edge.to];
            if (other_flag._flag_counter != counter) {
                other_flag = {1000000000, false, counter};
            }
            if (other_flag.dist > new_len) {
                other_flag.dist = new_len;
                other_flag.visited = true;
            }
        }
    }
}