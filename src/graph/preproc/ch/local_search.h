#pragma once

#include <algorithm>
#include <array>
#include <queue>
#include <tuple>
#include <vector>

#include "../../../algorithm/util.h"
#include "../../graph.h"
#include "../../structs/enums.h"
#include "../../structs/structs.h"
#include "./preproc_graph.h"

struct _FlagSH
{
    int curr_length;
    int curr_hops;
    int prev_edge;
    int prev_node;
    bool is_shortcut;
    bool visited;
    bool _is_target;
};

template <typename T>
bool contains(std::vector<T> vec, T item)
{
    if (std::find(vec.begin(), vec.end(), item) != vec.end()) {
        return true;
    } else {
        return false;
    }
}

//*******************************************
// ch utility
//*******************************************

// * searches for neighbours using edges and shortcuts for a node
//
// * is-contracted is used to limit search to nodes that have not been contracted yet (bool array containing every node in graph)
//
// * returns in-neighbours and out-neughbours
std::tuple<std::vector<int>, std::vector<int>> _FindNeighbours(CHPreprocGraph& explorer, int id, std::vector<bool> is_contracted)
{
    // compute out-going neighbours
    std::vector<int> out_neigbours;
    explorer.forAdjacentEdges(id, Direction::FORWARD, Adjacency::ADJACENT_ALL, [&id, &out_neigbours, &is_contracted](EdgeRef ref) {
        auto other_id = ref.other_id;
        if (other_id == id || contains(out_neigbours, other_id)) {
            return;
        }
        if (is_contracted[other_id]) {
            return;
        }
        out_neigbours.push_back(other_id);
    });

    // compute in-going neighbours
    std::vector<int> in_neigbours;
    explorer.forAdjacentEdges(id, Direction::BACKWARD, Adjacency::ADJACENT_ALL, [&id, &in_neigbours, &is_contracted](EdgeRef ref) {
        auto other_id = ref.other_id;
        if (other_id == id || contains(in_neigbours, other_id)) {
            return;
        }
        if (is_contracted[other_id]) {
            return;
        }
        in_neigbours.push_back(other_id);
    });

    return std::make_tuple(in_neigbours, out_neigbours);
}

// Performs a local dijkstra search from start until all targets are found or hop_limit reached.
// Flags will be set in flags-array.
// is_contracted contains true for every node that is already contracted (will not be used while finding shortest path).
void _RunLocalSearch(int start, std::vector<int>& targets, CHPreprocGraph& explorer, std::priority_queue<pq_item>& heap, Flags<_FlagSH>& flags, std::vector<bool> is_contracted,
                     int hop_limit)
{
    auto& start_flag = flags[start];
    start_flag.curr_length = 0;
    int target_count = targets.size();
    for (auto target : targets) {
        flags[target] = {
            .curr_length = 1000000000,
            ._is_target = true,
        };
    }
    start_flag.curr_length = 0;
    heap.push({start, 0});

    int found_count = 0;
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
        if (curr_flag._is_target) {
            found_count += 1;
        }
        if (found_count >= target_count) {
            break;
        }
        if (curr_flag.curr_hops >= hop_limit) {
            continue;
        }
        explorer.forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_ALL, [&is_contracted, &flags, &explorer, &curr_flag, curr_id, &heap](EdgeRef ref) {
            int edge_id = ref.edge_id;
            int other_id = ref.other_id;
            if (is_contracted[other_id]) {
                return;
            }
            auto& other_flag = flags[other_id];
            int weight = explorer.getEdgeWeight(ref);
            int newlength = curr_flag.curr_length + weight;
            if (newlength < other_flag.curr_length) {
                other_flag.curr_length = newlength;
                other_flag.curr_hops = curr_flag.curr_hops + 1;
                other_flag.prev_edge = edge_id;
                other_flag.prev_node = curr_id;
                other_flag.is_shortcut = ref.isShortcut();
                heap.push({other_id, newlength});
            }
        });
    }
}

// Returns the neccessary shortcut between from and to.
// If no shortcut is needed false will be returned.
std::tuple<std::array<std::tuple<int, bool>, 2>, bool> _GetShortcut(int from, int to, int via, CHPreprocGraph& explorer, Flags<_FlagSH> flags)
{
    std::array<std::tuple<int, bool>, 2> edges;

    auto to_flag = flags[to];
    // is target hasnt been found by search always add shortcut
    if (!to_flag.visited) {
        auto [t_edge, _] = explorer.getEdgeBetween(via, to);
        if (t_edge.isCHShortcut()) {
            edges[0] = std::make_tuple(t_edge.edge_id, true);
        } else {
            edges[0] = std::make_tuple(t_edge.edge_id, false);
        }
        auto [f_edge, _] = explorer.getEdgeBetween(from, via);
        if (f_edge.isCHShortcut()) {
            edges[1] = std::make_tuple(f_edge.edge_id, true);
        } else {
            edges[1] = std::make_tuple(f_edge.edge_id, false);
        }
        return std::make_tuple(edges, true);
    } else {
        // check if shortest path goes through node
        if (to_flag.prev_node != via) {
            return std::make_tuple(edges, false);
        }
        auto node_flag = flags[via];
        if (node_flag.prev_node != from) {
            return std::make_tuple(edges, false);
        }

        // capture edges that form shortcut
        if (to_flag.is_shortcut) {
            edges[0] = std::make_tuple(to_flag.prev_edge, true);
        } else {
            edges[0] = std::make_tuple(to_flag.prev_edge, false);
        }
        if (node_flag.is_shortcut) {
            edges[1] = std::make_tuple(node_flag.prev_edge, true);
        } else {
            edges[1] = std::make_tuple(node_flag.prev_edge, false);
        }
        return std::make_tuple(edges, true);
    }
}
