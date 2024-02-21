#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <stack>
#include <vector>

#include "../base/graph_base.h"
#include "../base/graph_index.h"
#include "../base/id_mapping.h"
#include "../base_graph.h"
#include "../graph.h"
#include "../speed_ups/ch_data.h"
#include "../speed_ups/partition.h"
#include "../weights/weighting.h"
#include "./tiled/util.h"

// builds node_mapping for _reorder_nodes functions
// multiple orderings can be applied in different orders (e.g. DFS + Levels) before building mapping
class MappingBuilder
{
private:
    std::vector<std::array<int, 2>> indices;

public:
    MappingBuilder(int node_count);

    // will update internal mapping with order-values
    // small order-values will be pushed to front and vice versa
    // uses stable_sort => equal order-values will not affect current order
    // does nothing if node_count does not match
    //
    // order_values contains value for node with id i (value = order_values[i])
    void updateOrdering(std::vector<int>& order_values);

    // updates internal mapping
    // id_mapping is used to map internal mapping ids to ids in order-values
    // if from_source is true => internal ids are target and order-value ids are source
    //
    // order_values contains value for node with id i (value = order_values[i])
    // if from_source: node_mapping is build for target nodes (node-ids of e.g. ch-data) but order_values are given for ordering of base thus id-mapping is used for conversion
    // if !from_source: internal mapping is build for graph-base and order-values are given for e.g. ch-data
    void updateOrdering(std::vector<int>& order_values, _IDMapping& id_mapping, bool from_source);

    std::vector<int> buildMapping() const;
};

static std::vector<int> _level_order_values(const CHData& ch)
{
    std::vector<int> order_values(ch.node_levels.size());
    for (int i = 0; i < ch.node_levels.size(); i++) {
        order_values[i] = -ch.node_levels[i];
    }
    return order_values;
}
static std::vector<int> _tile_order_values(IGraph& g, const Partition& partition)
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
static std::vector<int> _dfs_order_values(IGraph& g)
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

// default dfs-ordering for graph-base, weighting, index and partition
static std::vector<int> build_base_order(std::shared_ptr<GraphBase> base)
{
    auto weight = build_weighting(*base);
    auto index = build_base_index(*base);
    auto graph = build_base_graph(base, weight, index);

    MappingBuilder builder(base->nodeCount());
    auto order_values = _dfs_order_values(graph);
    builder.updateOrdering(order_values);
    return builder.buildMapping();
}

// level-ordering for chdata (build without partition)
// returns mapping for ch-data (not base)
static std::vector<int> build_ch_order(std::shared_ptr<GraphBase> base, std::shared_ptr<CHData> ch, std::shared_ptr<_IDMapping> id_mapping)
{
    auto weight = build_weighting(*base);
    auto index = build_base_index(*base);
    auto graph = build_base_graph(base, weight, index);

    MappingBuilder builder(base->nodeCount());
    auto order_values = _dfs_order_values(graph);
    builder.updateOrdering(order_values, *id_mapping, true);
    auto level_values = _level_order_values(*ch);
    builder.updateOrdering(level_values);
    return builder.buildMapping();
}

// tile+level-ordering for chdata2 (build with partition)
// returns mapping for ch-data (not base or partition)
static std::vector<int> build_ch2_order(std::shared_ptr<GraphBase> base, std::shared_ptr<Partition> partition, std::shared_ptr<CHData> ch, std::shared_ptr<_IDMapping> id_mapping)
{
    auto weight = build_weighting(*base);
    auto index = build_base_index(*base);
    auto graph = build_base_graph(base, weight, index);

    MappingBuilder builder(base->nodeCount());
    auto order_values = _dfs_order_values(graph);
    builder.updateOrdering(order_values, *id_mapping, true);
    auto level_values = _level_order_values(*ch);
    builder.updateOrdering(level_values);
    auto tile_values = _tile_order_values(graph, *partition);
    builder.updateOrdering(tile_values, *id_mapping, true);
    return builder.buildMapping();
}

// remaps mapping using id_mapping => applying original and created mapping will result in identity mapping between base and speed-up
// if to_target is true => mapping will be transformed from source-mapping to target-mapping
static std::vector<int> build_mapped_mapping(const std::vector<int>& mapping, const _IDMapping& id_mapping, bool to_target)
{
    std::vector<int> new_mapping(mapping.size());
    for (int i = 0; i < mapping.size(); i++) {
        int old_id = i;
        int new_id = mapping[i];
        int n_old_id;
        if (to_target) {
            n_old_id = id_mapping.get_target(old_id);
        } else {
            n_old_id = id_mapping.get_source(old_id);
        }
        new_mapping[n_old_id] = new_id;
    }
    return new_mapping;
}

// remaps mapping to identity mapping
// if to_target is true => resulting mapping will map source nodes (base) to target ordering
static std::vector<int> build_identity_mapping(const _IDMapping& id_mapping, bool to_target)
{
    int node_count = id_mapping.mapping.size();
    std::vector<int> new_mapping(node_count);
    for (int i = 0; i < node_count; i++) {
        int old_id;
        if (to_target) {
            int old_id = id_mapping.get_source(i);
        } else {
            int old_id = id_mapping.get_target(i);
        }
        int new_id = i;
        new_mapping[old_id] = new_id;
    }
    return new_mapping;
}
