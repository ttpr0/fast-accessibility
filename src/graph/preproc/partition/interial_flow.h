#pragma once

#include <math.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <span>
#include <vector>

#include "../../graph.h"
#include "./edmonds_karp.h"

#define PI 3.14159265358979323846

std::vector<int> SortNodes(IGraph& g, std::function<float(Coord)> coord_func)
{
    std::vector<int> nodes(g.nodeCount());
    for (int i = 0; i < g.nodeCount(); i++) {
        nodes[i] = i;
    }

    std::sort(nodes.begin(), nodes.end(), [&g, &coord_func](int& node_a, int& node_b) {
        auto coord_a = g.getNodeGeom(node_a);
        float val_a = coord_func(coord_a);
        auto coord_b = g.getNodeGeom(node_b);
        float val_b = coord_func(coord_b);
        return val_a < val_b;
    });

    return nodes;
}

std::vector<std::vector<int>> CreateOrders(IGraph& g)
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
    std::vector<std::vector<int>> orders(4);
    for (auto f : factors) {
        orders.push_back(SortNodes(g, [&f](Coord coord) { return f[0] * coord.lon + f[1] * coord.lat; }));
    }
    return orders;
}

std::vector<short> InertialFlow(IGraph& g)
{
    // compute node orderings by location embedding
    auto orders = CreateOrders(g);

    // create node_tiles array
    std::vector<short> node_tiles(g.nodeCount());
    for (int i = 0; i < g.nodeCount(); i++) {
        node_tiles[i] = 0;
    }

    // init processing queue containing to be splitted tiles
    std::queue<short> proc_queue;
    proc_queue.push(0);

    // keep track of tile ids
    short max_tile = 0;

    // iterate until no more tiles to be processed
    while (proc_queue.size() > 0) {
        short curr_tile = proc_queue.back();
        proc_queue.pop();
        short source_tile = max_tile + 1;
        short sink_tile = max_tile + 2;

        // dont process if node count is small enough
        int node_count = 0;
        for (int i = 0; i < node_tiles.size(); i++) {
            if (node_tiles[i] == curr_tile) {
                node_count += 1;
            }
        }
        if (node_count < 10000) {
            continue;
        }

        // compute max-flow for every direction
        std::vector<short> tiles;
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
            auto alg = EdmondsKarp(g, {&nodes[0], so_c}, source_tile, {&nodes[si_c], nodes.size() - si_c}, sink_tile,
                                   {&nodes[so_c], si_c - so_c}, curr_tile);
            int flow = alg.ComputeMaxFlow();
            std::cout << "computed flow: " << flow << std::endl;

            // select minimum max-flow
            if (flow < max_flow || max_flow == -1) {
                max_flow = flow;
                auto& t = alg.GetNodeTiles();
                tiles = std::move(t);
            }
        }
        // compute min-cut on minimum max-flow
        std::cout << "start computing min cut" << std::endl;

        // set computed tiles
        for (int i = 0; i < node_tiles.size(); i++) {
            if (node_tiles[i] != curr_tile) {
                continue;
            }
            node_tiles[i] = tiles[i];
        }
        max_tile += 2;

        // add new tiles to processing queue
        proc_queue.push(source_tile);
        proc_queue.push(sink_tile);
    }

    std::cout << "inertial flow finished" << std::endl;
    return node_tiles;
}
