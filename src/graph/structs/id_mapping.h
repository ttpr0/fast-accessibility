#pragma once

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

    _IDMapping(int size)
    {
        std::vector<std::array<int, 2>> mapping(size);
        for (int i = 0; i < size; i++) {
            mapping[i] = {i, i};
        }
        this->mapping = mapping;
    }
    _IDMapping(std::vector<std::array<int, 2>> mapping) : mapping(mapping) {}

    int get_target(int source) { return this->mapping[source][0]; }

    int get_source(int target) { return this->mapping[target][1]; }
};
