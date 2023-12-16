#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./util.h"

// standard RPHAST subset selection
std::vector<CHEdge> preprocessRPHAST(ICHGraph* g, std::queue<int>&& node_queue)
{
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
        g->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &node_queue](EdgeRef ref) {
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

// RPHAST subset selection using priority queue (RTS)
std::vector<CHEdge> preprocessRangeRPHAST(ICHGraph* g, std::priority_queue<pq_item>&& node_queue, int max_range)
{
    std::vector<bool> graph_subset(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        graph_subset[i] = false;
    }

    // select graph subset by marking visited nodes
    while (true) {
        if (node_queue.empty()) {
            break;
        }
        auto item = node_queue.top();
        int node = item.node;
        int node_len = item.dist;
        node_queue.pop();
        if (graph_subset[node]) {
            continue;
        }
        graph_subset[node] = true;
        g->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &g, &node_len, &max_range, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            int new_len = node_len + g->getEdgeWeight(ref);
            if (new_len > max_range) {
                return;
            }
            node_queue.push({ref.other_id, new_len});
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

// RPHAST preprocessing (including updates on dummy edges)
std::vector<CHEdge4> preprocessGSRPHAST(CHGraph2* g, std::queue<int>&& node_queue)
{
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
        g->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            node_queue.push(ref.other_id);
        });
    }
    // selecting subset of downward edges for linear sweep
    std::vector<CHEdge4> down_edges_subset;
    const std::vector<CHEdge4>& down_edges = g->getDownEdges4(Direction::FORWARD);
    int curr_id = 0;
    int curr_count = 0;
    down_edges_subset.push_back(down_edges[0]);
    for (int i = 0; i < down_edges.size(); i++) {
        auto edge = down_edges[i];
        if (edge.is_dummy) {
            if (curr_count == 0) {
                down_edges_subset[curr_id] = edge;
                curr_count = 0;
                continue;
            }
            down_edges_subset[curr_id].to = curr_count;
            curr_id = down_edges_subset.size();
            curr_count = 0;
            down_edges_subset.push_back(edge);
            continue;
        }
        if (!graph_subset[edge.from]) {
            continue;
        }
        down_edges_subset.push_back(edge);
        curr_count += 1;
    }
    down_edges_subset[curr_id].to = curr_count;

    return std::move(down_edges_subset);
}

// Range-RPHAST preprocessing (including updates on dummy edges)
std::vector<CHEdge4> preprocessRangeGSRPHAST(CHGraph2* g, std::priority_queue<pq_item>&& node_queue, int max_range)
{
    std::vector<bool> graph_subset(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        graph_subset[i] = false;
    }
    while (true) {
        if (node_queue.size() == 0) {
            break;
        }
        auto item = node_queue.top();
        int node = item.node;
        int node_len = item.dist;
        node_queue.pop();
        if (graph_subset[node]) {
            continue;
        }
        graph_subset[node] = true;
        g->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &g, &node_len, &max_range, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            int new_len = node_len + g->getEdgeWeight(ref);
            if (new_len > max_range) {
                return;
            }
            node_queue.push({ref.other_id, new_len});
        });
    }
    // selecting subset of downward edges for linear sweep
    std::vector<CHEdge4> down_edges_subset;
    const std::vector<CHEdge4>& down_edges = g->getDownEdges4(Direction::FORWARD);
    int curr_id = 0;
    int curr_count = 0;
    down_edges_subset.push_back(down_edges[0]);
    for (int i = 0; i < down_edges.size(); i++) {
        auto edge = down_edges[i];
        if (edge.is_dummy) {
            if (curr_count == 0) {
                down_edges_subset[curr_id] = edge;
                curr_count = 0;
                continue;
            }
            down_edges_subset[curr_id].to = curr_count;
            curr_id = down_edges_subset.size();
            curr_count = 0;
            down_edges_subset.push_back(edge);
            continue;
        }
        if (!graph_subset[edge.from]) {
            continue;
        }
        down_edges_subset.push_back(edge);
        curr_count += 1;
    }
    down_edges_subset[curr_id].to = curr_count;

    return std::move(down_edges_subset);
}
