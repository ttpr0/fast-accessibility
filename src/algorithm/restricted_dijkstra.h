#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./pq_item.h"

std::vector<int> calcRestrictedDijkstra(IGraph* g, int start, std::vector<bool>& targets)
{
    std::vector<int> dist(g->nodeCount());
    std::vector<bool> visited(g->nodeCount());
    int target_count;
    for (int i = 0; i < g->nodeCount(); i++) {
        dist[i] = 1000000000;
        visited[i] = false;
        if (targets[i]) {
            target_count += 1;
        }
    }
    dist[start] = 0;

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
        if (visited[curr_id]) {
            continue;
        }
        visited[curr_id] = true;
        if (targets[curr_id]) {
            found_count += 1;
            if (found_count == target_count) {
                break;
            }
        }
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&dist, &visited, &explorer, &heap, &curr_id](EdgeRef ref) {
            if (ref.isShortcut()) {
                return;
            }
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        });
    }

    return dist;
}
