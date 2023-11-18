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

// isoGRASP + reGRASP
class GRASP
{
public:
    typedef ITiledGraph Graph;
    class Builder
    {
    private:
        Graph* graph;
        std::vector<bool> active_tiles;
        int max_range;

    public:
        Builder(Graph* graph) : graph(graph), active_tiles(graph->tileCount() + 2) {}
        void addMaxRange(int range) { this->max_range = range; }
        void addTarget(int id)
        {
            short tile = this->graph->getNodeTile(id);
            this->active_tiles[tile] = true;
        }
        GRASP build() { return GRASP(this->graph, this->max_range, std::move(this->active_tiles)); }
    };

private:
    Graph* graph;
    std::vector<bool> active_tiles;
    std::vector<bool> found_tiles;
    int max_range;
    GRASP(Graph* graph, int max_range, std::vector<bool> active_tiles) : graph(graph), max_range(max_range), active_tiles(active_tiles), found_tiles(active_tiles.size()) {}

public:
    void compute(int s_id, Flags<DistFlag>& flags)
    {
        for (int i = 0; i < found_tiles.size(); i++) {
            found_tiles[i] = false;
        }
        calcGRASP(this->graph, s_id, flags, this->max_range, this->active_tiles, this->found_tiles);
    }
};