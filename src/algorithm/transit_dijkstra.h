#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "../graph/transit_graph.h"
#include "../util/pq_item.h"
#include "./util.h"

void calcBaseRange(IGraph& g, std::vector<std::tuple<int, int>> starts, Flags<DistFlag>& node_flags, Flags<DistFlag>& edge_flags, int max_range);

struct TransitItem
{
    int time;
    int departure;
    int stop;
};

// computes one-to-many distances using forward-dijkstra and public-transit
void calcTransitDijkstra(TransitGraph& g, int start, Flags<DistFlag>& node_flags, Flags<DistFlag>& edge_flags, Flags<TransitFlag>& stop_flags, int max_range, int from, int to)
{
    // step 1: range-dijkstra from start
    calcBaseRange(g.getBaseGraph(), {{start, 0}}, node_flags, edge_flags, max_range);

    // step 2: transit-dijkstra from all found stops
    std::priority_queue<pq<TransitItem, int>> heap;
    for (int i = 0; i < g.stopCount(); i++) {
        int base_node = g.mapToBaseNode(i);
        if (base_node == -1) {
            continue;
        }
        auto& flag = node_flags[base_node];
        if (flag.dist > max_range) {
            continue;
        }
        int dist = flag.dist;
        heap.push({{to + dist, to, i}, to + dist});
        g.forAdjacentConnections(i, FORWARD, ADJACENT_ALL, [&g, &heap, i, dist, from, to, max_range](EdgeRef ref) {
            if (ref.isShortcut()) {
                return;
            }
            auto weights = g.getConnectionWeight(ref, from + dist, to + dist);
            for (auto& w : weights) {
                heap.push({{w.departure, w.departure - dist, i}, w.departure});
            }
        });
    }
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto [item, _] = heap.top();
        heap.pop();
        auto& curr_flag = stop_flags[item.stop];
        bool prune = false;
        for (auto& trip : curr_flag.trips) {
            int time = std::get<0>(trip);
            int departure = std::get<1>(trip);
            if (time <= item.time && departure >= item.departure) {
                prune = true;
                break;
            }
        }
        if (prune) {
            continue;
        }
        curr_flag.trips.push_back({item.time, item.departure});
        g.forAdjacentConnections(item.stop, FORWARD, ADJACENT_ALL, [&g, &heap, &item, max_range](EdgeRef ref) {
            int other_id = ref.other_id;
            if (ref.isShortcut()) {
                auto weight = g.getShortcutWeight(ref);
                int new_time = item.time + weight;
                heap.push({{new_time, item.departure, other_id}, new_time});
            } else {
                auto weight = g.getConnectionWeight(ref, item.time);
                if (!weight) {
                    return;
                }
                int new_time = weight->arrival;
                heap.push({{new_time, item.departure, other_id}, new_time});
            }
        });
    }

    // step 3: range-dijkstra from all stops
    std::vector<std::tuple<int, int>> starts;
    for (int i = 0; i < g.stopCount(); i++) {
        auto& flag = stop_flags[i];
        if (flag.trips.empty()) {
            continue;
        }
        int dist = 100000000;
        for (auto& trip : flag.trips) {
            int time = std::get<0>(trip);
            int departure = std::get<1>(trip);
            int d = time - departure;
            if (d < dist) {
                dist = d;
            }
        }
        int base_node = g.mapToBaseNode(i);
        starts.push_back({base_node, dist});
    }
    calcBaseRange(g.getBaseGraph(), starts, node_flags, edge_flags, max_range);
}

void calcBaseRange(IGraph& g, std::vector<std::tuple<int, int>> starts, Flags<DistFlag>& node_flags, Flags<DistFlag>& edge_flags, int max_range)
{
    std::priority_queue<pq_item> heap;
    for (auto [start, dist] : starts) {
        auto& start_flag = node_flags[start];
        start_flag.dist = dist;

        g.forAdjacentEdges(start, FORWARD, ADJACENT_EDGES, [&edge_flags, &node_flags, &g, &heap, &max_range, &dist](EdgeRef ref) {
            int edge_id = ref.edge_id;
            int next_node_id = ref.other_id;
            int edge_dist = g.getEdgeWeight(ref) + dist;
            if (edge_dist > max_range) {
                return;
            }
            auto& edge_flag = edge_flags[edge_id];
            edge_flag.dist = edge_dist;
            heap.push({edge_id, edge_dist});
            auto& node_flag = node_flags[next_node_id];
            if (edge_dist < node_flag.dist) {
                node_flag.dist = edge_dist;
            }
        });
    }

    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        int curr_dist = item.dist;
        heap.pop();
        auto& curr_flag = edge_flags[curr_id];
        if (curr_flag.dist < curr_dist) {
            continue;
        }
        auto curr_edge = g.getEdge(curr_id);
        auto curr_ref = EdgeRef{curr_id, curr_edge.nodeB, 0};
        g.forAdjacentEdges(curr_edge.nodeB, FORWARD, ADJACENT_EDGES, [&edge_flags, &node_flags, &g, &heap, &max_range, &curr_flag, &curr_ref, &curr_edge](EdgeRef ref) {
            int other_id = ref.edge_id;
            auto other_node_b = ref.other_id;
            auto& other_flag = edge_flags[other_id];
            int new_length = curr_flag.dist + g.getEdgeWeight(ref) + g.getTurnCost(curr_ref, curr_edge.nodeB, ref);
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
                }
            }
        });
    }
}
