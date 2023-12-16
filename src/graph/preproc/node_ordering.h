#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <stack>
#include <vector>

#include "../comps/id_mapping.h"
#include "../graph.h"
#include "../speed_ups/ch_data.h"
#include "../speed_ups/partition.h"
#include "./tiled/util.h"

// builds node_mapping for _reorder_nodes functions
// multiple orderings can be applied in different orders (e.g. DFS + Levels) before building mapping
class MappingBuilder
{
private:
    std::vector<std::array<int, 2>> indices;

public:
    MappingBuilder(int node_count) : indices(node_count)
    {
        for (int i = 0; i < this->indices.size(); i++) {
            indices[i] = {i, 0};
        }
    }

    // will update internal mapping with order-values
    // small order-values will be pushed to front and vice versa
    // uses stable_sort => equal order-values will not affect current order
    // does nothing if node_count does not match
    void updateOrdering(std::vector<int>& order_values)
    {
        if (order_values.size() != this->indices.size()) {
            return;
        }
        for (int i = 0; i < this->indices.size(); i++) {
            int id = this->indices[i][0];
            this->indices[i][1] = order_values[id];
        }

        std::stable_sort(this->indices.begin(), this->indices.end(), [](const std::array<int, 2>& a, const std::array<int, 2>& b) {
            int val_a = a[1];
            int val_b = b[1];
            return val_a < val_b;
        });
    }

    // updates internal mapping
    // id_mapping is used to map internal mapping ids to ids in order-values
    // if from_source is true => internal ids are target and order-value ids are source
    void updateOrdering(std::vector<int>& order_values, _IDMapping& id_mapping, bool from_source)
    {
        if (order_values.size() != this->indices.size()) {
            return;
        }
        for (int i = 0; i < this->indices.size(); i++) {
            int id = this->indices[i][0];
            int m_id;
            if (from_source) {
                m_id = id_mapping.get_source(id);
            } else {
                m_id = id_mapping.get_target(id);
            }
            this->indices[i][1] = order_values[m_id];
        }

        std::stable_sort(this->indices.begin(), this->indices.end(), [](const std::array<int, 2>& a, const std::array<int, 2>& b) {
            int val_a = a[1];
            int val_b = b[1];
            return val_a < val_b;
        });
    }

    std::vector<int> buildMapping()
    {
        std::vector<int> mapping(this->indices.size());
        for (int i = 0; i < this->indices.size(); i++) {
            int new_id = i;
            int id = this->indices[i][0];
            mapping[id] = new_id;
        }
        return mapping;
    }
};

std::vector<int> _level_order_values(CHData& ch)
{
    std::vector<int> order_values(ch.node_levels.size());
    for (int i = 0; i < ch.node_levels.size(); i++) {
        order_values[i] = -ch.node_levels[i];
    }
    return order_values;
}

std::vector<int> _tile_order_values(IGraph& g, Partition& partition)
{
    std::vector<int> order_values(partition.node_tiles.size());
    auto is_border = _get_is_border(g, partition);
    for (int i = 0; i < partition.node_tiles.size(); i++) {
        int tile = partition.node_tiles[i];
        if (is_border[i]) {
            tile = -10000;
        }
        order_values[i] = tile;
    }
    return order_values;
}

std::vector<int> _dfs_order_values(IGraph& g)
{
    std::vector<int> order_values(g.nodeCount(), -1);

    std::stack<int> stack;
    stack.push(0);

    int curr_val = 0;
    while (!stack.empty()) {
        int curr = stack.top();
        stack.pop();

        if (order_values[curr] != -1) {
            continue;
        }
        order_values[curr] = curr_val;
        curr_val += 1;

        g.forAdjacentEdges(curr, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&stack, &order_values](EdgeRef ref) {
            if (order_values[ref.other_id] != -1) {
                return;
            }
            stack.push(ref.other_id);
        });
    }

    return order_values;
}
