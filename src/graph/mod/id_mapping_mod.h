#pragma once

#include <memory>
#include <vector>

#include "../base/id_mapping.h"

// reorders sources,
// mapping: old id -> new id
std::shared_ptr<_IDMapping> _reorder_sources(const _IDMapping& id_mapping, const std::vector<int>& mapping);

// reorders targets,
// mapping: old id -> new id
std::shared_ptr<_IDMapping> _reorder_targets(const _IDMapping& id_mapping, const std::vector<int>& mapping);
