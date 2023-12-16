#pragma once

#include <vector>

#include "../structs/adjacency.h"
#include "../structs/structs.h"

class TiledData
{
public:
    std::vector<Shortcut> shortcuts;
    AdjacencyArray topology;
    std::vector<char> edge_types;

    char getEdgeType(int edge) const { return this->edge_types[edge]; }
    int shortcutCount() const { return this->shortcuts.size(); }
    Shortcut getShortcut(int shc) const { return this->shortcuts[shc]; }
};
