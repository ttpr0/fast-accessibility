#pragma once

#include <vector>

#include "../structs/adjacency.h"
#include "../structs/structs.h"

class CHData
{
public:
    std::vector<Shortcut> shortcuts;
    AdjacencyArray topology;
    std::vector<short> node_levels;

    short getNodeLevel(int node) const { return this->node_levels[node]; }

    int shortcutCount() const { return this->shortcuts.size(); }

    Shortcut getShortcut(int shc) const { return this->shortcuts[shc]; }
};
