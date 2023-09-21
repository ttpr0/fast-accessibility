#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./pq_item.h"

std::vector<int> calcRPHAST(ICHGraph* g, int start, std::vector<bool>& targets)
{
    std::queue<int> node_queue;
    std::vector<int> dist(g->nodeCount());
    std::vector<bool> visited(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        dist[i] = 1000000000;
        visited[i] = false;
        if (targets[i]) {
            node_queue.push(i);
        }
    }
    dist[start] = 0;

    auto explorer = g->getGraphExplorer();
    std::vector<bool> graph_subset(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        graph_subset[i] = false;
    }
    while (true) {
        if (node_queue.size() == 0) {
            break;
        }
        auto node = node_queue.front();
        node_queue.pop();
        if (graph_subset[node]) {
            continue;
        }
        graph_subset[node] = true;
        short node_level = g->getNodeLevel(node);
        explorer->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            node_queue.push(ref.other_id);
        });
    }
    // selecting subset of downward edges for linear sweep
    std::vector<CHEdge> down_edges_subset;
    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    for (int i = 0; i < down_edges.size(); i++) {
        auto edge = down_edges[i];
        if (!graph_subset[edge.from]) {
            continue;
        }
        down_edges_subset.push_back(edge);
    }

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
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
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &curr_id](EdgeRef ref) {
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

    int length = down_edges_subset.size();
    for (int i = 0; i < length; i++) {
        auto edge = down_edges_subset[i];
        int curr_len = dist[edge.from];
        int new_len = curr_len + edge.weight;
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }

    return dist;
}
