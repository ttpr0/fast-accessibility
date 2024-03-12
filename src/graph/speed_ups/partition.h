#pragma once

#include <array>
#include <unordered_set>
#include <vector>

#include "../structs/enums.h"

//*******************************************
// partition class
//*******************************************

class Partition
{
public:
    std::vector<short> node_tiles;

    short get_node_tile(int node) const { return this->node_tiles[node]; }

    short tile_count() const
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

    std::vector<short> get_tiles() const
    {
        std::unordered_set<short> tile_set;
        for (int i = 0; i < this->node_tiles.size(); i++) {
            short tile_id = this->node_tiles[i];
            if (tile_set.contains(tile_id)) {
                continue;
            }
            tile_set.insert(tile_id);
        }
        std::vector<short> tile_list;
        for (auto tile : tile_set) {
            tile_list.push_back(tile);
        }
        return tile_list;
    }
};
