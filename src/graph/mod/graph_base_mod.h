#pragma once

#include <memory>
#include <vector>

#include "../base/graph_base.h"

std::shared_ptr<GraphBase> _remove_nodes(const GraphBase& base, const std::vector<int>& nodes);

// reorders nodes,
// mapping: old id -> new id
std::shared_ptr<GraphBase> _reorder_nodes(const GraphBase& base, const std::vector<int>& mapping);
