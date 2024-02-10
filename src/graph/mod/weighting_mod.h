#pragma once

#include <memory>
#include <vector>

#include "../weights/tc_weighting.h"
#include "../weights/weighting.h"

// reorders nodes,
// mapping: old id -> new id
std::shared_ptr<Weighting> _reorder_nodes(const Weighting& weight, const std::vector<int>& mapping);

// reorders nodes,
// mapping: old id -> new id
std::shared_ptr<TCWeighting> _reorder_nodes(const TCWeighting& weight, const std::vector<int>& mapping);
