#pragma once

#include <array>
#include <memory>
#include <vector>

#include "../structs/enums.h"
#include "./graph_base.h"

//*******************************************
// weighting class
//*******************************************

class Weighting
{
public:
    std::vector<int> edge_weights;

    int get_edge_weight(int edge) { return this->edge_weights[edge]; }

    int get_turn_cost(int from, int via, int to) { return 0; }
};

static std::shared_ptr<Weighting> build_default_weighting(GraphBase& base)
{
    std::vector<int> weights(base.edgeCount());
    for (int i = 0; i < base.edgeCount(); i++) {
        auto edge = base.edges[i];
        int w = edge.length * 3.6 / edge.maxspeed;
        if (w < 1) {
            w = 1;
        }
        weights[i] = w;
    }

    return std::make_shared<Weighting>(std::move(weights));
}

static std::shared_ptr<Weighting> build_equal_weighting(GraphBase& base)
{
    std::vector<int> weights(base.edgeCount());
    for (int i = 0; i < base.edgeCount(); i++) {
        weights[i] = 1;
    }

    return std::make_shared<Weighting>(std::move(weights));
}
