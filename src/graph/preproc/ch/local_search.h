#pragma once

#include <algorithm>
#include <array>
#include <queue>
#include <tuple>
#include <vector>

#include "../../../util/flags.h"
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
};

template <typename T>
bool contains(std::vector<T>& vec, T item)
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
static void _find_neighbours(const CHPreprocGraph& graph, int id, std::vector<int>& in_neigbours, std::vector<int>& out_neigbours)
{
    // compute out-going neighbours
    out_neigbours.clear();
    graph.forAdjacentEdges(id, Direction::FORWARD, Adjacency::ADJACENT_ALL, [&id, &out_neigbours, &graph](EdgeRef ref) {
        auto other_id = ref.other_id;
        if (other_id == id) {
            return;
        }
        if (contains(out_neigbours, other_id)) {
            return;
        }
        if (graph.isContracted(other_id)) {
            return;
        }
        out_neigbours.push_back(other_id);
    });

    // compute in-going neighbours
    in_neigbours.clear();
    graph.forAdjacentEdges(id, Direction::BACKWARD, Adjacency::ADJACENT_ALL, [&id, &in_neigbours, &graph](EdgeRef ref) {
        auto other_id = ref.other_id;
        if (other_id == id) {
            return;
        }
        if (contains(in_neigbours, other_id)) {
            return;
        }
        if (graph.isContracted(other_id)) {
            return;
        }
        in_neigbours.push_back(other_id);
    });
}

// Performs a local dijkstra search from start until all targets are found or hop_limit reached.
// Flags will be set in flags-array.
// is_contracted contains true for every node that is already contracted (will not be used while finding shortest path).
static void _run_local_search(int contracted, int start, const CHPreprocGraph& graph, std::priority_queue<pq_item>& heap, Flags<_FlagSH>& flags, int hop_limit)
{
    // find range limit
    int max_weight = 0;
    graph.forAdjacentEdges(contracted, FORWARD, ADJACENT_ALL, [&max_weight, &flags, &graph](EdgeRef ref) {
        int other_id = ref.other_id;
        if (graph.isContracted(other_id)) {
            return;
        }
        int weight = graph.getEdgeWeight(ref);
        if (weight > max_weight) {
            max_weight = weight;
        }
    });
    int max_length = graph.getWeightBetween(start, contracted) + max_weight;

    // init search
    auto& start_flag = flags[start];
    start_flag.curr_length = 0;
    start_flag.curr_hops = 0;
    heap.push({start, 0});

    // run search
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
        if (curr_flag.curr_hops >= hop_limit) {
            continue;
        }
        graph.forAdjacentEdges(curr_id, FORWARD, ADJACENT_ALL, [&flags, &graph, &curr_flag, curr_id, &heap, &max_length, &contracted](EdgeRef ref) {
            int edge_id = ref.edge_id;
            int other_id = ref.other_id;
            if (graph.isContracted(other_id)) {
                return;
            }
            int weight = graph.getEdgeWeight(ref);
            int newlength = curr_flag.curr_length + weight;
            if (newlength > max_length) {
                return;
            }
            auto& other_flag = flags[other_id];
            // update shortest path if shorter is found, prefer shorter paths avoiding contracted node
            if (newlength < other_flag.curr_length || (newlength == other_flag.curr_length && other_flag.prev_node == contracted)) {
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
static std::tuple<std::array<std::tuple<int, bool>, 2>, bool> _get_shortcut(int from, int to, int via, const CHPreprocGraph& explorer, Flags<_FlagSH>& flags)
{
    std::array<std::tuple<int, bool>, 2> edges;

    auto to_flag = flags[to];
    // check if target hasnt been found by search
    if (!to_flag.visited) {
        return std::make_tuple(edges, false);
    }
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
