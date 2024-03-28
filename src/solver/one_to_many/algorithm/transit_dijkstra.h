#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../../../graph/graph.h"
#include "../../../graph/transit_graph.h"
#include "../../../util/flags.h"
#include "../../../util/pq_item.h"
#include "../../../util/snap.h"
#include "./many_dijkstra.h"

struct TransitItem
{
    int time;
    int departure;
    int stop;
};

// computes one-to-many distances using forward-dijkstra and public-transit
void calcTransitDijkstra(TransitGraph& g, DSnap start, Flags<DistFlag>& node_flags, Flags<DistFlag>& edge_flags, Flags<TransitFlag>& stop_flags, int max_range, int from, int to)
{
    // step 1: range-dijkstra from start
    std::vector<Snap> starts;
    for (int i = 0; i < start.len(); i++) {
        starts.push_back({start[i].node, start[i].dist});
    }
    calcMultiDijkstraTC(g.getBaseGraph(), starts, node_flags, edge_flags, max_range);

    // step 2: transit-dijkstra from all found stops
    std::priority_queue<pq<TransitItem, int>> heap;
    for (int i = 0; i < g.stopCount(); i++) {
        auto base_snap = g.mapToBaseNode(i);
        if (base_snap.len() == 0) {
            continue;
        }
        int dist = 100000000;
        for (int j = 0; j < base_snap.len(); j++) {
            auto snap = base_snap[j];
            auto& flag = node_flags[snap.node];
            int new_dist = snap.dist + flag.dist;
            if (new_dist < dist) {
                dist = new_dist;
            }
        }
        if (dist > max_range) {
            continue;
        }
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
    starts.clear();
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
        auto base_snap = g.mapToBaseNode(i);
        for (int j = 0; j < base_snap.len(); j++) {
            auto snap = base_snap[j];
            starts.push_back({snap.node, dist + snap.dist});
        }
    }
    calcMultiDijkstraTC(g.getBaseGraph(), starts, node_flags, edge_flags, max_range);
}
