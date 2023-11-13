#pragma once

#include <array>
#include <vector>

#include "../structs/enums.h"

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
