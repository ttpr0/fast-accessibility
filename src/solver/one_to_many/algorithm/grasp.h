#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../../../graph/graph.h"
#include "../../../util/flags.h"
#include "../../../util/pq_item.h"
#include "../../../util/snap.h"

// reGRASP and isoGRASP combination
template <typename TGraph>
    requires any_grasp_graph<TGraph>
void calcGRASP(TGraph& g, DSnap start, Flags<DistFlag>& flags, int max_range, std::vector<bool>& contains_targets, std::vector<bool>& is_found)
{
    std::priority_queue<pq_item> heap;
    short start_tile;
    for (int i = 0; i < start.len(); i++) {
        auto s = start[i];
        auto& start_flag = flags[s.node];
        start_flag.dist = s.dist;

        start_tile = g.getNodeTile(s.node);
        is_found[start_tile] = true;

        heap.push({s.node, s.dist});
    }

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
        short curr_tile = g.getNodeTile(curr_id);
        auto handler = [&flags, &g, &heap, &max_range, &curr_flag](EdgeRef ref) {
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
        };
        if (curr_tile == start_tile) {
            g.forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, handler);
        } else {
            is_found[curr_tile] = true;
            g.forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_SKIP, handler);
        }
    }
    for (int i = 0; i < contains_targets.size(); i++) {
        if (!contains_targets[i] || !is_found[i]) {
            continue;
        }
        auto& down_edges = g.getIndexEdges(i, Direction::FORWARD);
        for (int j = 0; j < down_edges.size(); j++) {
            auto edge = down_edges[j];
            auto curr_flag = flags[edge.from];
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
}
