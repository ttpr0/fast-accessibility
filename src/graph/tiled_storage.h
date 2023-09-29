#pragma once

#include <vector>

#include "./structs.h"

class TiledStore
{
public:
    std::vector<Shortcut> shortcuts;
    std::vector<std::tuple<int, char>> edge_refs;
    std::vector<short> node_tiles;
    std::vector<int> sh_weights;
    std::vector<char> edge_types;

    TiledStore(std::vector<Shortcut> shortcuts, std::vector<std::tuple<int, char>> edge_refs, std::vector<short> tiles, std::vector<int> weights, std::vector<char> edge_types)
        : shortcuts(shortcuts), edge_refs(edge_refs), node_tiles(tiles), sh_weights(weights), edge_types(edge_types)
    {}

    short getNodeTile(int node) { return this->node_tiles[node]; }
    char getEdgeType(int edge) { return this->edge_types[edge]; }
    short tileCount()
    {
        short max = 0;
        for (int i = 0; i < this->node_tiles.size(); i++) {
            short tile = this->node_tiles[i];
            if (tile > max) {
                max = tile;
            }
        }
        return max + 1;
    }
    int shortcutCount() { return this->shortcuts.size(); }
    Shortcut getShortcut(int shc) { return this->shortcuts[shc]; }
};
