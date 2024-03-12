#pragma once

#include <vector>

#include "../structs/adjacency.h"

// reorders nodes,
// mapping: old id -> new id
AdjacencyArray _reorder_nodes(const AdjacencyArray& tiled, const std::vector<int>& mapping);
