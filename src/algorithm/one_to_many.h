#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./pq_item.h"

void calcRestrictedDijkstra(IGraph* g, int start, std::vector<int>& dist, std::vector<bool>& visited, std::vector<bool>& targets, int target_count)
{
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
}

void calcPHAST(ICHGraph* g, int start, std::vector<int>& dist, std::vector<bool>& visited)
{
    dist[start] = 0;

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

    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    int length = down_edges.size();
    for (int i = 0; i < length; i++) {
        auto edge = down_edges[i];
        int curr_len = dist[edge.from];
        int new_len = curr_len + edge.weight;
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }
}

void calcRPHAST(ICHGraph* g, int start, std::vector<int>& dist, std::vector<bool>& visited, std::vector<CHEdge>& down_edges_subset)
{
    dist[start] = 0;

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
}

std::vector<CHEdge> preprocessRPHAST(ICHGraph* g, std::vector<bool>& targets)
{
    std::queue<int> node_queue;
    for (int i = 0; i < g->nodeCount(); i++) {
        if (targets[i]) {
            node_queue.push(i);
        }
    }

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

    return std::move(down_edges_subset);
}