#pragma once

#include <memory>
#include <vector>

#include "../speed_ups/ch_data.h"
#include "../speed_ups/ch_index.h"

// reorders nodes,
// mapping: old id -> new id
std::shared_ptr<CHData> _reorder_nodes(const CHData& ch, const std::vector<int>& mapping);

// reorders nodes,
// mapping: old id -> new id
std::shared_ptr<_CHIndex> _reorder_nodes(const _CHIndex& index, const std::vector<int>& mapping);
