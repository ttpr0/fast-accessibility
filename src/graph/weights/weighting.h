#pragma once

#include <array>
#include <memory>
#include <tuple>
#include <vector>

#include "../base/graph_base.h"

//*******************************************
// weighting class
//*******************************************

class Weighting
{
public:
    std::vector<int> edge_weights;

    int get_edge_weight(int edge) const;
    void set_edge_weight(int edge, int weight);
};

std::shared_ptr<Weighting> build_weighting(const GraphBase& base);
std::shared_ptr<Weighting> build_default_weighting(const GraphBase& base);
