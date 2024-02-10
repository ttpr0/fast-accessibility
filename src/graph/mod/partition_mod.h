#pragma once

#include <memory>
#include <vector>

#include "../speed_ups/partition.h"

// reorders nodes,
// mapping: old id -> new id
std::shared_ptr<Partition> _reorder_nodes(const Partition& partition, const std::vector<int>& mapping);
