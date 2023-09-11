#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./pq_item.h"

std::vector<int> calcAllDijkstra(IGraph* g, int start, int max_range)
{
    std::vector<int> dist(g->nodeCount());
    std::vector<bool> visited(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        dist[i] = 1000000000;
        visited[i] = false;
    }
    dist[start] = 0;

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});

    auto explorer = g->getGraphExplorer();

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
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_ALL, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
            if (ref.isShortcut()) {
                return;
            }
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        });
    }

    return dist;
}
