#pragma once

#include <memory>
#include <vector>

#include "../speed_ups/partition.h"

// reorders nodes,
// mapping: old id -> new id
static std::shared_ptr<Partition> _reorder_nodes(const Partition& partition, const std::vector<int>& mapping)
{
    std::vector<short> new_tiles(partition.node_tiles.size());
    for (int i = 0; i < partition.node_tiles.size(); i++) {
        int m_id = mapping[i];
        new_tiles[m_id] = partition.get_node_tile(i);
    }

    return std::make_shared<Partition>(new_tiles);
}
