#pragma once

#include <memory>
#include <vector>

#include "../comps/weighting.h"

// reorders nodes,
// mapping: old id -> new id
static std::shared_ptr<Weighting> _reorder_nodes(const Weighting& weight, const std::vector<int>& mapping)
{
    std::vector<int> new_weights = weight.edge_weights; 

    return std::make_shared<Weighting>(std::move(new_weights));
}

// reorders nodes,
// mapping: old id -> new id
static std::shared_ptr<TCWeighting> _reorder_nodes(const TCWeighting& weight, const std::vector<int>& mapping)
{
    std::vector<int> edge_weights = weight.edge_weights;
    std::vector<std::tuple<char, char>> edge_indices = weight.edge_indices;

    // compute order (reversed mapping)
    std::vector<int> order(mapping.size());
    for (int i = 0; i < mapping.size(); i++) {
        order[mapping[i]] = i;
    }
    std::vector<std::tuple<int, char, char>> turn_refs(weight.turn_refs.size());
    std::vector<char> turn_costs(weight.turn_weights.size());
    int loc = 0;
    for (int i = 0; i < order.size(); i++) {
        int old_id = order[i];
        int new_id = i;
        auto [l, r, c] = weight.turn_refs[old_id];
        turn_refs[new_id] = std::make_tuple(loc, r, c);
        // compute size of tc matrix
        int count = (int)r * c;
        // copy over elements of matrix
        for (int j = 0; j < count; j++) {
            turn_costs[loc + j] = weight.turn_weights[l + j];
        }
        // set start loc for next iteration
        loc += count;
    }

    return std::make_shared<TCWeighting>(std::move(edge_weights), std::move(edge_indices), std::move(turn_refs), std::move(turn_costs));
}
