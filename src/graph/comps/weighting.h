#pragma once

#include <array>
#include <memory>
#include <tuple>
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

    int get_edge_weight(int edge) const { return this->edge_weights[edge]; }
    int set_edge_weight(int edge, int weight) { this->edge_weights[edge] = weight; }
};

static std::shared_ptr<Weighting> build_default_weighting(const GraphBase& base)
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

static std::shared_ptr<Weighting> build_equal_weighting(const GraphBase& base)
{
    std::vector<int> weights(base.edgeCount());
    for (int i = 0; i < base.edgeCount(); i++) {
        weights[i] = 1;
    }

    return std::make_shared<Weighting>(std::move(weights));
}

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

    int get_edge_weight(int edge) const { return this->edge_weights[edge]; }
    int set_edge_weight(int edge, int weight) { this->edge_weights[edge] = weight; }

    int get_turn_cost(int from, int via, int to) const
    {
        char bwd_index = std::get<1>(this->edge_indices[from]);
        char fwd_index = std::get<0>(this->edge_indices[to]);
        auto [loc, rows, cols] = this->turn_refs[via];
        return this->turn_weights[loc + (int)cols * bwd_index + fwd_index];
    }
    void set_turn_cost(int from, int via, int to, int weight)
    {
        char bwd_index = std::get<1>(this->edge_indices[from]);
        char fwd_index = std::get<0>(this->edge_indices[to]);
        auto [loc, rows, cols] = this->turn_refs[via];
        this->turn_weights[loc + (int)cols * bwd_index + fwd_index] = weight;
    }
};

static std::shared_ptr<TCWeighting> build_default_tc_weighting(const GraphBase& base)
{
    std::vector<int> edge_weights(base.edgeCount());
    for (int i = 0; i < base.edgeCount(); i++) {
        auto edge = base.edges[i];
        int w = edge.length * 3.6 / edge.maxspeed;
        if (w < 1) {
            w = 1;
        }
        edge_weights[i] = w;
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
