#include <array>
#include <memory>
#include <tuple>
#include <vector>

#include "../structs/enums.h"
#include "./tc_weighting.h"

int TCWeighting::get_edge_weight(int edge) const
{
    return this->edge_weights[edge];
}
void TCWeighting::set_edge_weight(int edge, int weight)
{
    this->edge_weights[edge] = weight;
}

int TCWeighting::get_turn_cost(int from, int via, int to) const
{
    char bwd_index = std::get<1>(this->edge_indices[from]);
    char fwd_index = std::get<0>(this->edge_indices[to]);
    auto [loc, rows, cols] = this->turn_refs[via];
    return this->turn_weights[loc + (int)cols * bwd_index + fwd_index];
}
void TCWeighting::set_turn_cost(int from, int via, int to, int weight)
{
    char bwd_index = std::get<1>(this->edge_indices[from]);
    char fwd_index = std::get<0>(this->edge_indices[to]);
    auto [loc, rows, cols] = this->turn_refs[via];
    this->turn_weights[loc + (int)cols * bwd_index + fwd_index] = weight;
}

std::shared_ptr<TCWeighting> build_tc_weighting(const GraphBase& base)
{
    std::vector<int> edge_weights(base.edgeCount());
    for (int i = 0; i < base.edgeCount(); i++) {
        edge_weights[i] = 1;
    }

    std::vector<std::tuple<char, char>> edge_indices(base.edgeCount());
    std::vector<std::tuple<int, char, char>> turn_refs(base.nodeCount());
    int loc = 0;
    for (int i = 0; i < base.nodeCount(); i++) {
        char fwd_index = 0;
        auto accessor = base.adjacency.getNeighbours(i, Direction::FORWARD);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            std::get<0>(edge_indices[int(edge_id)]) = fwd_index;
            fwd_index += 1;
        }
        char bwd_index = 0;
        accessor = base.adjacency.getNeighbours(i, Direction::BACKWARD);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            std::get<1>(edge_indices[int(edge_id)]) = bwd_index;
            bwd_index += 1;
        }
        turn_refs[i] = std::make_tuple(loc, bwd_index, fwd_index);
        loc += (int)bwd_index * fwd_index;
    }
    std::vector<char> turn_costs(loc, 0);

    return std::make_shared<TCWeighting>(std::move(edge_weights), std::move(edge_indices), std::move(turn_refs), std::move(turn_costs));
}
