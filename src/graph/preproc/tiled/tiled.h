#pragma once

#include <algorithm>
#include <array>
#include <cstdio>
#include <memory>
#include <queue>
#include <tuple>
#include <vector>

#include "../../base_graph.h"
#include "../../graph.h"
#include "../../speed_ups/cell_index.h"
#include "../../speed_ups/partition.h"
#include "../../speed_ups/tiled_data.h"
#include "../../structs/enums.h"
#include "../../structs/structs.h"
#include "../ch/ch.h"
#include "./util.h"

// Creates tiled-graph with full-shortcut cliques.
static std::shared_ptr<TiledData> calc_clique_overlay(const GraphBase& base, const Weighting& weights, const Partition& partition)
{
    auto graph = BaseGraphExplorer(base, weights);

    // init components
    std::vector<Shortcut> skip_shortcuts;
    std::vector<char> edge_types(graph.edgeCount());

    // update cross border
    for (int i = 0; i < graph.edgeCount(); i++) {
        auto edge = graph.getEdge(i);
        short tileA = partition.get_node_tile(edge.nodeA);
        short tileB = partition.get_node_tile(edge.nodeB);
        if (tileA != tileB) {
            edge_types[i] = 10;
        }
    }

    // compute cliques
    auto tiles = partition.get_tiles();
    int tile_count = tiles.size();
    int c = 1;
    for (auto tile_id : tiles) {
        printf("tile %i: %i / %i \n", tile_id, c, tile_count);
        printf("tile %i: getting start nodes \n", tile_id);
        auto [start_nodes, end_nodes] = _get_inout_nodes(graph, partition, tile_id);
        printf("tile %i: calculating skip edges \n", tile_id);
        Flags<_Flag> flags(graph.nodeCount(), {10000000, -1, false});
        for (auto start : start_nodes) {
            flags.soft_reset();
            _calc_shortest_paths(graph, start, flags, partition);
            for (auto end : end_nodes) {
                auto& end_flag = flags[end];
                if (!end_flag.visited) {
                    continue;
                }
                int length = end_flag.pathlength;
                Shortcut shc = {
                    .from = start,
                    .to = end,
                    .weight = length,
                };
                skip_shortcuts.push_back(shc);
            }
        }
        printf("tile %i: finished \n", tile_id);
        c += 1;
    }

    // build
    auto skip_topology = _create_skip_topology(graph, skip_shortcuts, edge_types);

    return std::make_shared<TiledData>(std::move(skip_shortcuts), std::move(skip_topology), std::move(edge_types));
}

//
static std::shared_ptr<_CellIndex> calc_full_cell_index(const GraphBase& base, const Weighting& weights, const Partition& partition)
{
    auto graph = BaseGraphExplorer(base, weights);

    // init components
    std::unordered_map<short, std::vector<Shortcut>> fwd_index_edges;
    std::unordered_map<short, std::vector<Shortcut>> bwd_index_edges;

    // compute cell-indices
    auto tiles = partition.get_tiles();
    int ind = 0;
    for (auto tile : tiles) {
        ind += 1;
        std::cout << "Process Tile:" << ind << "/" << tiles.size() << std::endl;
        std::vector<Shortcut> index_edges;
        auto [b_nodes, i_nodes] = _get_border_interior_nodes(graph, partition, tile);
        Flags<_Flag> flags(graph.nodeCount(), {10000000, -1, false});
        for (auto b_node : b_nodes) {
            flags.soft_reset();
            _calc_shortest_paths(graph, b_node, flags, partition);
            for (auto i_node : i_nodes) {
                auto& i_flag = flags[i_node];
                if (!i_flag.visited) {
                    continue;
                }
                int length = i_flag.pathlength;
                Shortcut shc = {
                    .from = b_node,
                    .to = i_node,
                    .weight = length,
                };
                index_edges.push_back(shc);
            }
        }
        fwd_index_edges[tile] = index_edges;
    }

    return std::make_shared<_CellIndex>(fwd_index_edges, bwd_index_edges);
}

static std::tuple<std::shared_ptr<TiledData>, std::shared_ptr<_CellIndex>> calc_isophast_overlay(const GraphBase& base, const Weighting& weights, const Partition& partition)
{
    printf("Compute subset contraction: \n");
    auto ch_data = calc_partial_contraction(base, weights, partition);

    printf("Set border nodes to maxlevel: \n");
    auto graph = BaseGraphExplorer(base, weights);
    auto is_border = _get_is_border(graph, partition);
    short max_level = 0;
    auto& node_levels = ch_data->node_levels;
    for (int i = 0; i < node_levels.size(); i++) {
        if (node_levels[i] > max_level) {
            max_level = node_levels[i];
        }
    }
    for (int i = 0; i < node_levels.size(); i++) {
        if (is_border[i]) {
            node_levels[i] = max_level + 1;
        }
    }

    printf("Create topology from shortcuts: \n");
    // init components
    std::vector<char> edge_types(base.edgeCount());

    // update cross border
    for (int i = 0; i < graph.edgeCount(); i++) {
        auto edge = graph.getEdge(i);
        short tileA = partition.get_node_tile(edge.nodeA);
        short tileB = partition.get_node_tile(edge.nodeB);
        if (tileA != tileB) {
            edge_types[i] = 10;
        }
    }

    // extract overlay
    std::vector<Shortcut> skip_shortcuts;
    for (auto shc : ch_data->shortcuts) {
        if (is_border[shc.from] && is_border[shc.to]) {
            skip_shortcuts.push_back(shc);
        }
    }

    // build topology
    auto skip_topology = _create_skip_topology(graph, skip_shortcuts, edge_types);

    // build tiled data
    auto tiled_data = std::make_shared<TiledData>(std::move(skip_shortcuts), std::move(skip_topology), std::move(edge_types));

    printf("Create downwards edge lists: \n");

    // init components
    std::unordered_map<short, std::vector<Shortcut>> fwd_index_edges;
    std::unordered_map<short, std::vector<Shortcut>> bwd_index_edges;

    // compute cell indices
    auto tiles = partition.get_tiles();
    int ind = 0;
    for (auto tile : tiles) {
        ind += 1;
        printf("Process Tile: %i/%i \n", ind, (int)tiles.size());
        // get all down edges or shortcuts
        std::vector<Shortcut> index_edges;
        for (int i = 0; i < ch_data->shortcuts.size(); i++) {
            auto shc = ch_data->getShortcut(i);
            short from_tile = partition.get_node_tile(shc.from);
            short to_tile = partition.get_node_tile(shc.to);
            if (from_tile != tile || to_tile != tile) {
                continue;
            }
            if (ch_data->node_levels[shc.from] > ch_data->node_levels[shc.to]) {
                index_edges.push_back({
                    .from = shc.from,
                    .to = shc.to,
                    .weight = shc.weight,
                });
            }
        }
        for (int i = 0; i < graph.edgeCount(); i++) {
            auto edge = graph.getEdge(i);
            short from_tile = partition.get_node_tile(edge.nodeA);
            short to_tile = partition.get_node_tile(edge.nodeB);
            if (from_tile != tile || to_tile != tile) {
                continue;
            }
            if (ch_data->node_levels[edge.nodeA] > ch_data->node_levels[edge.nodeB]) {
                index_edges.push_back({
                    .from = edge.nodeA,
                    .to = edge.nodeB,
                    .weight = graph.getEdgeWeight({i, 0, 0}),
                });
            }
        }

        // sort down edges by node level
        std::sort(index_edges.begin(), index_edges.end(), [&node_levels](const Shortcut& shc_a, const Shortcut& shc_b) {
            auto level_a = node_levels[shc_a.from];
            auto level_b = node_levels[shc_b.from];
            return level_a > level_b;
        });

        // add edges to index_edges
        fwd_index_edges[tile] = index_edges;
    }

    auto cell_index = std::make_shared<_CellIndex>(fwd_index_edges, bwd_index_edges);

    return std::make_tuple(tiled_data, cell_index);
}
