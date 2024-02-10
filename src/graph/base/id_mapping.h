#pragma once

#include <array>
#include <memory>
#include <vector>

// Maps indices from a source to a target and reversed.
class _IDMapping
{
public:
    // Contains two mappings:
    //
    // -> first value maps source s to target t: mapping[s][0] = t
    //
    // -> second value maps target t to source s: mapping[t][1] = s
    std::vector<std::array<int, 2>> mapping;

    int get_target(int source) const { return this->mapping[source][0]; }

    int get_source(int target) const { return this->mapping[target][1]; }
};

static std::shared_ptr<_IDMapping> new_id_mapping(int size)
{
    std::vector<std::array<int, 2>> mapping(size);
    for (int i = 0; i < size; i++) {
        mapping[i] = {i, i};
    }
    return std::make_shared<_IDMapping>(mapping);
}
