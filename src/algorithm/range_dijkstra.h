#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./util.h"

// computes distances to nodes until "max_range" using dijkstra
void calcRangeDijkstra(IGraph* g, int start, Flags<DistFlag>& flags, int max_range)
{
    auto& start_flag = flags[start];
    start_flag.dist = 0;

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
        g->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&flags, &g, &heap, &max_range, &curr_flag](EdgeRef ref) {
            if (ref.isShortcut()) {
                return;
            }
            int other_id = ref.other_id;
            auto& other_flag = flags[other_id];
            if (other_flag.visited) {
                return;
            }
            int new_length = curr_flag.dist + g->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (other_flag.dist > new_length) {
                other_flag.dist = new_length;
                heap.push({other_id, new_length});
            }
        });
    }
}

void calcRangeDijkstraTC(IGraph* g, int start, Flags<DistFlag>& node_flags, Flags<DistFlag>& edge_flags, int max_range)
{
    auto& start_flag = node_flags[start];
    start_flag.dist = 0;

    std::priority_queue<pq_item> heap;
    g->forAdjacentEdges(start, FORWARD, ADJACENT_EDGES, [&edge_flags, &node_flags, &g, &heap, &max_range](EdgeRef ref) {
        int edge_id = ref.edge_id;
        int next_node_id = ref.other_id;
        int edge_dist = g->getEdgeWeight(ref);
        if (edge_dist > max_range) {
            return;
        }
        auto& edge_flag = edge_flags[edge_id];
        edge_flag.dist = edge_dist;
        heap.push({edge_id, edge_dist});
        auto& node_flag = node_flags[next_node_id];
        if (edge_dist < node_flag.dist) {
            node_flag.dist = edge_dist;
            node_flag.visited = true;
        }
    });

    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        auto& curr_flag = edge_flags[curr_id];
        if (curr_flag.visited) {
            continue;
        }
        curr_flag.visited = true;
        auto curr_edge = g->getEdge(curr_id);
        auto curr_ref = EdgeRef{curr_id, curr_edge.nodeB, 0};
        g->forAdjacentEdges(curr_edge.nodeB, FORWARD, ADJACENT_EDGES, [&edge_flags, &node_flags, &g, &heap, &max_range, &curr_flag, &curr_ref, &curr_edge](EdgeRef ref) {
            int other_id = ref.edge_id;
            auto other_node_b = ref.other_id;
            auto& other_flag = edge_flags[other_id];
            if (other_flag.visited) {
                return;
            }
            int new_length = curr_flag.dist + g->getEdgeWeight(ref) + g->getTurnCost(curr_ref, curr_edge.nodeB, ref);
            if (new_length > max_range) {
                return;
            }
            if (other_flag.dist > new_length) {
                auto& edge_flag = edge_flags[other_id];
                edge_flag.dist = new_length;
                heap.push({other_id, new_length});
                auto& node_flag = node_flags[other_node_b];
                if (new_length < node_flag.dist) {
                    node_flag.dist = new_length;
                    node_flag.visited = true;
                }
            }
        });
    }
}
