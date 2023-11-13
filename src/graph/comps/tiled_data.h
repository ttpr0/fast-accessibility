#pragma once

#include <vector>

#include "../structs/adjacency.h"
#include "../structs/id_mapping.h"
#include "../structs/structs.h"

class TiledData
{
public:
    _IDMapping id_mapping;
    std::vector<Shortcut> shortcuts;
    AdjacencyArray topology;
    std::vector<char> edge_types;

    char getEdgeType(int edge) { return this->edge_types[edge]; }
    int shortcutCount() { return this->shortcuts.size(); }
    Shortcut getShortcut(int shc) { return this->shortcuts[shc]; }
};
