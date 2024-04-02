#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>

#include "../../../util/pq_item.h"
#include "../tiled/util.h"
#include "./contraction_order.h"
#include "./local_search.h"
#include "./preproc_graph.h"

//*******************************************
// preprocess ch
//*******************************************

template <typename T>
    requires IContractionOrder<T>
std::shared_ptr<CHData> calc_contraction(const GraphBase& base, const Weighting& weights, function_ref<T(CHPreprocGraph&)> builder)
{
    auto graph = CHPreprocGraph(base, weights);

    std::cout << "started contracting graph..." << std::endl;
    T ordering = builder(graph);
    Flags<_FlagSH> flags(graph.nodeCount(), {.curr_length = 100000000});
    std::priority_queue<pq_item> heap;
    std::vector<int> in_neigbours;
    std::vector<int> out_neigbours;
    int hop_limit = ordering.get_hop_limit();

    std::cout << "start contracting nodes..." << std::endl;
    while (true) {
        int node_id = ordering.get_next();
        if (node_id == -1) {
            break;
        }

        // contract node
        short level = graph.getNodeLevel(node_id);
        hop_limit = ordering.get_hop_limit();
        _find_neighbours(graph, node_id, in_neigbours, out_neigbours);
        for (int i = 0; i < in_neigbours.size(); i++) {
            int from = in_neigbours[i];
            flags.soft_reset();
            _run_local_search(node_id, from, graph, heap, flags, hop_limit);
            for (int j = 0; j < out_neigbours.size(); j++) {
                int to = out_neigbours[j];
                if (from == to) {
                    continue;
                }
                auto [edges, shortcut_needed] = _get_shortcut(from, to, node_id, graph, flags);
                if (!shortcut_needed) {
                    continue;
                }
                // add shortcut to graph
                graph.addShortcut(from, to, edges);
                ordering.shortcut_added(from, to, edges);
            }
        }
        // set node to contracted
        graph.setContracted(node_id);

        // update neighbour levels
        for (int i = 0; i < in_neigbours.size(); i++) {
            int nb = in_neigbours[i];
            graph.setNodeLevel(nb, std::max<short>(level + 1, graph.getNodeLevel(nb)));
        }
        for (int i = 0; i < out_neigbours.size(); i++) {
            int nb = out_neigbours[i];
            graph.setNodeLevel(nb, std::max<short>(level + 1, graph.getNodeLevel(nb)));
        }

        ordering.node_contracted(node_id, in_neigbours, out_neigbours);
    }
    std::cout << "finished contracting graph" << std::endl;

    return graph.getCHData();
}

//*******************************************
// preprocessing methods
//*******************************************

// Computes contraction using node-degree ordering.
static std::shared_ptr<CHData> calc_simple_contraction(const GraphBase& base, const Weighting& weights)
{
    return calc_contraction<SimpleOrdering>(base, weights, [](CHPreprocGraph& graph) { return SimpleOrdering(graph); });
}

// Computes contraction using 2*ED + CN + EC + 5*L with hop-limits.
static std::shared_ptr<CHData> calc_phast_contraction(const GraphBase& base, const Weighting& weights)
{
    return calc_contraction<PHASTOrdering>(base, weights, [](CHPreprocGraph& graph) { return PHASTOrdering(graph); });
}

// Computes contraction using 2*ED + CN + EC + 5*L with hop-limits.
//
// Only non border nodes are contracted.
static std::shared_ptr<CHData> calc_partial_contraction(const GraphBase& base, const Weighting& weights, const Partition& partition)
{
    return calc_contraction<PartialPHASTOrdering>(base, weights, [&](CHPreprocGraph& graph) { return PartialPHASTOrdering(graph, partition); });
}

// Computes contraction using 2*ED + CN + EC + 5*L without hop-limits (uses partition to contract border nodes last).
static std::shared_ptr<CHData> calc_tiled_contraction(const GraphBase& base, const Weighting& weights, const Partition& partition)
{
    return calc_contraction<GSPHASTOrdering>(base, weights, [&](CHPreprocGraph& graph) { return GSPHASTOrdering(graph, partition); });
}
