#pragma once

#include <array>
#include <memory>
#include <tuple>
#include <vector>

#include "../base/graph_base.h"

//*******************************************
// weighting class with turn-costs
//*******************************************

class TCWeighting
{
public:
    std::vector<int> edge_weights;
    std::vector<std::tuple<char, char>> edge_indices;
    std::vector<std::tuple<int, char, char>> turn_refs;
    std::vector<char> turn_weights;

    int get_edge_weight(int edge) const;
    void set_edge_weight(int edge, int weight);

    int get_turn_cost(int from, int via, int to) const;
    void set_turn_cost(int from, int via, int to, int weight);
};

std::shared_ptr<TCWeighting> build_tc_weighting(const GraphBase& base);
std::shared_ptr<TCWeighting> build_default_tc_weighting(const GraphBase& base);
