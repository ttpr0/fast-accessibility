#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./pq_item.h"

std::vector<int> calcRangePHAST(ICHGraph* g, int start, int max_range)
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
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
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

    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    int length = down_edges.size();
    for (int i = 0; i < length; i++) {
        auto edge = down_edges[i];
        int curr_len = dist[edge.from];
        if (curr_len > max_range) {
            continue;
        }
        int new_len = curr_len + edge.weight;
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }

    return dist;
}

std::vector<int> calcRangePHAST2(ICHGraph* g, int start, int max_range)
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
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
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

    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    int length = down_edges.size();
    for (int i = 0; i < length; ++i) {
        auto edge = down_edges[i];
        int curr_len = dist[edge.from];
        if (curr_len > max_range) {
            i += edge.count - 1;
            continue;
        }
        int new_len = curr_len + edge.weight;
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }

    return dist;
}

std::vector<int> calcRangePHAST3(ICHGraph* g, int start, int max_range)
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
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
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

    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    bool skip = false;
    int count = 0;
    int i = 0;
    int length = down_edges.size();
    while (i < length) {
        if (skip) {
            count -= 1;
            if (count == 0) {
                skip = false;
                i -= 1;
            }
            i += 1;
            continue;
        }
        auto edge = down_edges[i];
        int curr_len = dist[edge.from];
        if (curr_len > max_range) {
            count = edge.count;
            skip = true;
            i += 1;
            continue;
        }
        int new_len = curr_len + edge.weight;
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
        i += 1;
    }

    return dist;
}

std::vector<int> calcRangePHAST4(ICHGraph* g, int start, int max_range)
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
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
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

    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    int length_1 = 100000;
    int length = down_edges.size();
    for (int i = 0; i < length_1; ++i) {
        auto edge = down_edges[i];
        int curr_len = dist[edge.from];
        if (curr_len > max_range) {
            i += edge.count - 1;
            continue;
        }
        int new_len = curr_len + edge.weight;
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }
    for (int i = length_1; i < length; ++i) {
        auto edge = down_edges[i];
        int curr_len = dist[edge.from];
        if (curr_len > max_range) {
            continue;
        }
        int new_len = curr_len + edge.weight;
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }

    return dist;
}
