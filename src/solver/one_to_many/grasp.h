#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "../../algorithm/grasp.h"
#include "../../algorithm/util.h"
#include "../../graph/graph.h"
#include "../../graph/tiled_graph.h"
#include "../../util.h"
#include "./state.h"

// isoGRASP + reGRASP
class GRASP
{
private:
    bool is_build;
    ITiledGraph* graph;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;

public:
    GRASP(ITiledGraph* graph) : graph(graph), max_range(max_range), active_tiles(graph->tileCount() + 2, false), found_tiles(graph->tileCount() + 2, false)
    {
        this->is_build = false;
        this->max_range = 10000000;
    }

    void addMaxRange(int range)
    {
        if (this->is_build) {
            return;
        }
        this->max_range = range;
    }
    void addTarget(int id)
    {
        if (this->is_build) {
            return;
        }
        short tile = this->graph->getNodeTile(id);
        this->active_tiles[tile] = true;
    }

    bool isBuild() { return this->is_build; }
    void build()
    {
        if (this->is_build) {
            return;
        }
        this->is_build = true;
    }

    NodeBasedState makeComputeState() { return NodeBasedState(this->graph->nodeCount()); }
    void compute(int s_id, NodeBasedState& state)
    {
        if (!this->is_build) {
            return;
        }
        for (int i = 0; i < found_tiles.size(); i++) {
            found_tiles[i] = false;
        }
        calcGRASP(this->graph, s_id, state.flags, this->max_range, this->active_tiles, this->found_tiles);
    }
};
