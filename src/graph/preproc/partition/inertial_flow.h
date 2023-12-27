#pragma once

#include <math.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <queue>
#include <span>
#include <vector>

#include "../../../util/pq_item.h"
#include "../../explorer/base_explorer.h"
#include "./edmonds_karp.h"

#define PI 3.14159265358979323846

static std::vector<int> _sort_nodes(const GraphBase& base, std::function<float(Coord)> coord_func)
{
    std::vector<int> nodes(base.nodeCount());
    for (int i = 0; i < base.nodeCount(); i++) {
        nodes[i] = i;
    }

    std::sort(nodes.begin(), nodes.end(), [&base, &coord_func](int& node_a, int& node_b) {
        auto coord_a = base.getNodeGeom(node_a);
        float val_a = coord_func(coord_a);
        auto coord_b = base.getNodeGeom(node_b);
        float val_b = coord_func(coord_b);
        return val_a < val_b;
    });

    return nodes;
}

static std::vector<std::vector<int>> _create_orders(const GraphBase& base)
{
    float sin_45 = sin(PI / 4);
    float cos_45 = cos(PI / 4);
    std::vector<std::array<float, 2>> factors = {
        // vertical
        {1, 0},
        // horizontal
        {0, 1},
        // diagonal ll to ur
        {-sin_45, cos_45},
        // diagonal ul to lr
        {sin_45, cos_45},
    };
    std::vector<std::vector<int>> orders;
    for (auto f : factors) {
        orders.push_back(_sort_nodes(base, [&f](Coord coord) { return f[0] * coord.lon + f[1] * coord.lat; }));
    }
    return orders;
}

static std::vector<short> inertial_flow(const GraphBase& base, const Weighting& weight, int tile_count)
{
    // compute node orderings by location embedding
    auto orders = _create_orders(base);

    BaseGraphExplorer expl = {base, weight};

    // create node_tiles array
    std::vector<short> node_tiles(base.nodeCount());
    for (int i = 0; i < base.nodeCount(); i++) {
        node_tiles[i] = 0;
    }

    // init processing queue containing to be splitted tiles
    std::priority_queue<pq<short, int>> proc_queue;
    proc_queue.push({0, -base.nodeCount()});

    // keep track of tile ids
    short max_tile = 0;

    // iterate until no more tiles to be processed
    while (proc_queue.size() > 0) {
        auto temp = proc_queue.top();
        short curr_tile = temp.val;
        proc_queue.pop();
        short source_tile = curr_tile;
        short sink_tile = max_tile + 1;

        // dont process if node count is small enough
        int node_count = 0;
        for (int i = 0; i < node_tiles.size(); i++) {
            if (node_tiles[i] == curr_tile) {
                node_count += 1;
            }
        }

        // compute max-flow for every direction
        EdmondsKarp max_alg;
        int max_flow = -1;
        std::cout << "start computing flows" << std::endl;
        for (auto order : orders) {
            // select nodes from current tile
            std::vector<int> nodes;
            for (auto node : order) {
                if (node_tiles[node] == curr_tile) {
                    nodes.push_back(node);
                }
            }
            // create source and sink thresholds
            float k = 0.25;
            int so_c = nodes.size() * k;
            int si_c = nodes.size() * (1 - k);

            // compute max-flow for current direction
            auto alg = EdmondsKarp(&expl, {&nodes[0], (size_t)so_c}, source_tile, {&nodes[si_c], nodes.size() - si_c}, sink_tile, {&nodes[so_c], (size_t)(si_c - so_c)}, -1);
            int flow = alg.ComputeMaxFlow();
            std::cout << "computed flow: " << flow << std::endl;

            // select minimum max-flow
            if (flow < max_flow || max_flow == -1) {
                max_flow = flow;
                max_alg = std::move(alg);
            }
        }
        // compute min-cut on minimum max-flow
        std::cout << "start computing min cut" << std::endl;
        max_alg.ComputeMinCut();

        // set computed tiles
        std::vector<short> tiles = max_alg.GetNodeTiles();
        int source_count = 0;
        int sink_count = 0;
        for (int i = 0; i < node_tiles.size(); i++) {
            if (node_tiles[i] != curr_tile) {
                continue;
            }
            if (tiles[i] == source_tile) {
                source_count += 1;
            }
            if (tiles[i] == sink_tile) {
                sink_count += 1;
            }
            node_tiles[i] = tiles[i];
        }
        max_tile += 1;
        if (max_tile >= tile_count) {
            break;
        }

        // add new tiles to processing queue
        proc_queue.push({source_tile, -source_count});
        proc_queue.push({sink_tile, -sink_count});
    }

    std::cout << "inertial flow finished" << std::endl;
    return node_tiles;
}
