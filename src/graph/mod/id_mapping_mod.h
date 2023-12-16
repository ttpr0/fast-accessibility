#pragma once

#include <array>
#include <memory>

#include "../comps/id_mapping.h"

// reorders sources,
// mapping: old id -> new id
static std::shared_ptr<_IDMapping> _reorder_sources(const _IDMapping& id_mapping, const std::vector<int>& mapping)
{
    if (id_mapping.mapping.size() != mapping.size()) {
        throw -1;
    }
    std::vector<std::array<int, 2>> new_id_mapping(id_mapping.mapping.size());
    for (int i = 0; i < id_mapping.mapping.size(); i++) {
        auto map = id_mapping.mapping[i];
        int s = i;
        int t = map[0];
        int new_s = mapping[s];

        // remap source to target mapping
        new_id_mapping[new_s][0] = t;

        // remap target to source mapping
        new_id_mapping[t][1] = new_s;
    }

    return std::make_shared<_IDMapping>(new_id_mapping);
}

// reorders targets,
// mapping: old id -> new id
static std::shared_ptr<_IDMapping> _reorder_targets(const _IDMapping& id_mapping, const std::vector<int>& mapping)
{
    if (id_mapping.mapping.size() != mapping.size()) {
        throw -1;
    }
    std::vector<std::array<int, 2>> new_id_mapping(id_mapping.mapping.size());
    for (int i = 0; i < id_mapping.mapping.size(); i++) {
        auto map = id_mapping.mapping[i];
        int t = i;
        int s = map[1];
        int new_t = mapping[t];

        // remap source to target mapping
        new_id_mapping[new_t][1] = s;

        // remap target to source mapping
        new_id_mapping[s][0] = new_t;
    }

    return std::make_shared<_IDMapping>(new_id_mapping);
}
