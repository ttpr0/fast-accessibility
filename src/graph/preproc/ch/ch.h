#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <queue>
#include <tuple>
#include <vector>

#include "../tiled/util.h"
#include "./local_search.h"
#include "./preproc_graph.h"

//*******************************************
// preprocess ch
//*******************************************

class CHOrdering;

template <typename T, typename P>
struct pq
{
    T val;
    P prio;
};

template <typename T, typename P>
bool operator<(const pq<T, P>& a, const pq<T, P>& b)
{
    return a.prio > b.prio;
}

template <typename T>
T max(T a, T b)
{
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int _ComputeNodePriority(int node, CHPreprocGraph& graph, Flags<_FlagSH>& flags, std::vector<bool>& is_contracted, std::vector<short>& node_levels,
                         std::vector<int>& contracted_neighbours, std::vector<char>& shortcut_edgecount, int hop_limit)
{
    auto [in_neigbours, out_neigbours] = _FindNeighbours(graph, node, is_contracted);
    int edge_diff = -(in_neigbours.size() + out_neigbours.size());
    char edge_count = 0;
    for (int i = 0; i < in_neigbours.size(); i++) {
        int from = in_neigbours[i];
        flags.soft_reset();
        std::priority_queue<pq_item> heap;
        _RunLocalSearch(from, out_neigbours, graph, heap, flags, is_contracted, hop_limit);
        for (int j = 0; j < out_neigbours.size(); j++) {
            int to = out_neigbours[j];
            if (from == to) {
                continue;
            }
            auto [edges, shortcut_needed] = _GetShortcut(from, to, node, graph, flags);
            if (!shortcut_needed) {
                continue;
            }
            edge_diff += 1;
            // compute number of edges representing the shortcut (limited to 3)
            char ec = 0;
            if (!std::get<1>(edges[0])) {
                ec += 1;
            } else {
                ec += shortcut_edgecount[std::get<0>(edges[0])];
            }
            if (!std::get<1>(edges[1])) {
                ec += 1;
            } else {
                ec += shortcut_edgecount[std::get<0>(edges[1])];
            }
            if (ec > 3) {
                ec = 3;
            }
            edge_count += ec;
        }
    }

    return 2 * edge_diff + contracted_neighbours[node] + int(edge_count) + 5 * int(node_levels[node]);
}

//*******************************************
// preprocess ch
//*******************************************

// Computes contraction using 2*ED + CN + EC + 5*L with hop-limits.
std::shared_ptr<CHData> CalcContraction3(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights)
{
    auto graph = CHPreprocGraph(base, weights);

    std::cout << "started contracting graph..." << std::endl;

    // initialize
    std::vector<bool> is_contracted(graph.nodeCount());
    std::vector<short> node_levels(graph.nodeCount());
    std::vector<int> contracted_neighbours(graph.nodeCount());
    std::vector<char> shortcut_edgecount;
    for (int i = 0; i < graph.nodeCount(); i++) {
        is_contracted[i] = false;
        node_levels[i] = 0;
        contracted_neighbours[i] = 0;
    }

    // initialize routing components
    int node_count = graph.nodeCount();
    int edge_count = graph.edgeCount();
    Flags<_FlagSH> flags(graph.nodeCount(), {.curr_length = 100000000});
    int hop_limit = 5;

    // compute node priorities
    std::cout << "computing priorities..." << std::endl;
    std::vector<int> node_priorities(graph.nodeCount());
    for (int i = 0; i < graph.nodeCount(); i++) {
        node_priorities[i] = _ComputeNodePriority(i, graph, flags, is_contracted, node_levels, contracted_neighbours, shortcut_edgecount, hop_limit);
    }

    // put nodes into priority queue
    std::priority_queue<pq<std::tuple<int, int>, int>> contraction_order;
    for (int i = 0; i < graph.nodeCount(); i++) {
        int prio = node_priorities[i];
        contraction_order.push({std::make_tuple(i, prio), prio});
    }

    std::cout << "start contracting nodes..." << std::endl;
    int count = 0;
    while (true) {
        if (contraction_order.empty()) {
            break;
        }
        auto temp = contraction_order.top();
        contraction_order.pop();

        int node_id = std::get<0>(temp.val);
        int node_prio = std::get<1>(temp.val);
        if (is_contracted[node_id] || node_prio != node_priorities[node_id]) {
            continue;
        }
        node_count -= 1;
        count += 1;
        if (count % 1000 == 0) {
            std::cout << "	node :" << count << "/" << graph.nodeCount() << std::endl;
        }

        // contract node
        short level = node_levels[node_id];
        auto [in_neigbours, out_neigbours] = _FindNeighbours(graph, node_id, is_contracted);
        int ed = in_neigbours.size() + out_neigbours.size();
        for (int i = 0; i < in_neigbours.size(); i++) {
            int from = in_neigbours[i];
            std::priority_queue<pq_item> heap;
            flags.soft_reset();
            _RunLocalSearch(from, out_neigbours, graph, heap, flags, is_contracted, hop_limit);
            for (int j = 0; j < out_neigbours.size(); j++) {
                int to = out_neigbours[j];
                if (from == to) {
                    continue;
                }
                auto [edges, shortcut_needed] = _GetShortcut(from, to, node_id, graph, flags);
                if (!shortcut_needed) {
                    continue;
                }
                // add shortcut to graph
                graph.AddShortcut(from, to, edges);
                ed -= 1;

                // compute number of edges representing the shortcut (limited to 3)
                char ec = 0;
                if (!std::get<1>(edges[0])) {
                    ec += 1;
                } else {
                    ec += shortcut_edgecount[(!std::get<0>(edges[0]))];
                }
                if (!std::get<1>(edges[1])) {
                    ec += 1;
                } else {
                    ec += shortcut_edgecount[(!std::get<0>(edges[1]))];
                }
                if (ec > 3) {
                    ec = 3;
                }
                shortcut_edgecount.push_back(ec);
            }
        }
        edge_count -= ed;
        if (node_count > 0) {
            if (edge_count * 2 / node_count > 5) {
                hop_limit = 10;
            }
            if (edge_count * 2 / node_count > 10) {
                hop_limit = 10000000;
            }
        }
        // set node to contracted
        is_contracted[node_id] = true;

        // update neighbours
        for (int i = 0; i < in_neigbours.size(); i++) {
            int nb = in_neigbours[i];
            node_levels[nb] = max<short>(level + 1, node_levels[nb]);
            contracted_neighbours[nb] += 1;
            int prio = _ComputeNodePriority(nb, graph, flags, is_contracted, node_levels, contracted_neighbours, shortcut_edgecount, hop_limit);
            node_priorities[nb] = prio;
            contraction_order.push({std::make_tuple(nb, prio), prio});
        }
        for (int i = 0; i < out_neigbours.size(); i++) {
            int nb = out_neigbours[i];
            node_levels[nb] = max<short>(level + 1, node_levels[nb]);
            contracted_neighbours[nb] += 1;
            int prio = _ComputeNodePriority(nb, graph, flags, is_contracted, node_levels, contracted_neighbours, shortcut_edgecount, hop_limit);
            node_priorities[nb] = prio;
            contraction_order.push({std::make_tuple(nb, prio), prio});
        }
    }
    for (int i = 0; i < graph.nodeCount(); i++) {
        graph.setNodeLevel(i, node_levels[i]);
    }
    std::cout << "finished contracting graph" << std::endl;

    return graph.getCHData();
}

// Computes contraction using 2*ED + CN + EC + 5*L with hop-limits.
//
// Only non border nodes are contracted.
std::shared_ptr<CHData> CalcPartialContraction3(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights, std::shared_ptr<Partition> partition)
{
    auto graph = CHPreprocGraph(base, weights);

    std::cout << "started contracting graph..." << std::endl;

    // initialize
    std::vector<bool> is_contracted(graph.nodeCount());
    std::vector<short> node_levels(graph.nodeCount());
    std::vector<int> contracted_neighbours(graph.nodeCount());
    std::vector<char> shortcut_edgecount;
    for (int i = 0; i < graph.nodeCount(); i++) {
        is_contracted[i] = false;
        node_levels[i] = 0;
        contracted_neighbours[i] = 0;
    }

    // initialize routing components
    int node_count = graph.nodeCount();
    int edge_count = graph.edgeCount();
    Flags<_FlagSH> flags(graph.nodeCount(), {.curr_length = 100000000});
    int hop_limit = 5;

    // compute node priorities
    std::cout << "computing priorities..." << std::endl;
    Graph b_graph = build_base_graph(base, weights);
    auto is_border = _get_is_border(b_graph, *partition);
    std::vector<int> node_priorities(graph.nodeCount());
    std::priority_queue<pq<std::tuple<int, int>, int>> contraction_order;
    for (int i = 0; i < graph.nodeCount(); i++) {
        if (is_border[i]) {
            node_priorities[i] = 10000000000;
            continue;
        }
        int prio = _ComputeNodePriority(i, graph, flags, is_contracted, node_levels, contracted_neighbours, shortcut_edgecount, hop_limit);
        node_priorities[i] = prio;
        contraction_order.push({std::make_tuple(i, prio), prio});
    }

    std::cout << "start contracting nodes..." << std::endl;
    int count = 0;
    while (true) {
        if (contraction_order.empty()) {
            break;
        }
        auto temp = contraction_order.top();
        contraction_order.pop();

        int node_id = std::get<0>(temp.val);
        int node_prio = std::get<1>(temp.val);
        if (is_contracted[node_id] || node_prio != node_priorities[node_id]) {
            continue;
        }
        node_count -= 1;
        count += 1;
        if (count % 1000 == 0) {
            std::cout << "	node :" << count << "/" << graph.nodeCount() << std::endl;
        }

        // contract node
        short level = node_levels[node_id];
        auto [in_neigbours, out_neigbours] = _FindNeighbours(graph, node_id, is_contracted);
        int ed = in_neigbours.size() + out_neigbours.size();
        for (int i = 0; i < in_neigbours.size(); i++) {
            int from = in_neigbours[i];
            std::priority_queue<pq_item> heap;
            flags.soft_reset();
            _RunLocalSearch(from, out_neigbours, graph, heap, flags, is_contracted, hop_limit);
            for (int j = 0; j < out_neigbours.size(); j++) {
                int to = out_neigbours[j];
                if (from == to) {
                    continue;
                }
                auto [edges, shortcut_needed] = _GetShortcut(from, to, node_id, graph, flags);
                if (!shortcut_needed) {
                    continue;
                }
                // add shortcut to graph
                graph.AddShortcut(from, to, edges);
                ed -= 1;

                // compute number of edges representing the shortcut (limited to 3)
                char ec = 0;
                if (!std::get<1>(edges[0])) {
                    ec += 1;
                } else {
                    ec += shortcut_edgecount[(!std::get<0>(edges[0]))];
                }
                if (!std::get<1>(edges[1])) {
                    ec += 1;
                } else {
                    ec += shortcut_edgecount[(!std::get<0>(edges[1]))];
                }
                if (ec > 3) {
                    ec = 3;
                }
                shortcut_edgecount.push_back(ec);
            }
        }
        edge_count -= ed;
        if (node_count > 0) {
            if (edge_count * 2 / node_count > 5) {
                hop_limit = 10;
            }
            if (edge_count * 2 / node_count > 10) {
                hop_limit = 10000000;
            }
        }
        // set node to contracted
        is_contracted[node_id] = true;

        // update neighbours
        for (int i = 0; i < in_neigbours.size(); i++) {
            int nb = in_neigbours[i];
            node_levels[nb] = max<short>(level + 1, node_levels[nb]);
            contracted_neighbours[nb] += 1;
            if (is_border[nb]) {
                continue;
            }
            int prio = _ComputeNodePriority(nb, graph, flags, is_contracted, node_levels, contracted_neighbours, shortcut_edgecount, hop_limit);
            node_priorities[nb] = prio;
            contraction_order.push({std::make_tuple(nb, prio), prio});
        }
        for (int i = 0; i < out_neigbours.size(); i++) {
            int nb = out_neigbours[i];
            node_levels[nb] = max<short>(level + 1, node_levels[nb]);
            contracted_neighbours[nb] += 1;
            if (is_border[nb]) {
                continue;
            }
            int prio = _ComputeNodePriority(nb, graph, flags, is_contracted, node_levels, contracted_neighbours, shortcut_edgecount, hop_limit);
            node_priorities[nb] = prio;
            contraction_order.push({std::make_tuple(nb, prio), prio});
        }
    }
    for (int i = 0; i < graph.nodeCount(); i++) {
        graph.setNodeLevel(i, node_levels[i]);
    }
    std::cout << "finished contracting graph" << std::endl;

    return graph.getCHData();
}
