#pragma once

#include <memory>
#include <vector>

#include "../speed_ups/cell_index.h"
#include "../speed_ups/tiled_data.h"

// reorders nodes,
// mapping: old id -> new id
std::shared_ptr<TiledData> _reorder_nodes(const TiledData& tiled, const std::vector<int>& mapping);

// reorders nodes,
// mapping: old id -> new id
std::shared_ptr<_CellIndex> _reorder_nodes(const _CellIndex& index, const std::vector<int>& mapping);
