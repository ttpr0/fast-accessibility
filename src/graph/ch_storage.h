#pragma once

#include <vector>

#include "./structs.h"

class CHStore
{
public:
    std::vector<CHShortcut> shortcuts;
    std::vector<short> node_levels;
    std::vector<int> sh_weights;

    CHStore(std::vector<CHShortcut> shortcuts, std::vector<short> levels, std::vector<int> weights)
        : shortcuts(shortcuts), node_levels(levels), sh_weights(weights)
    {}

    short getNodeLevel(int node) {
        return this->node_levels[node];
    }

    int shortcutCount() {
        return this->shortcuts.size();
    }

    CHShortcut getShortcut(int shc) {
        return this->shortcuts[shc];
    }
};
