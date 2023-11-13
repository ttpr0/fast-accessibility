#pragma once

#include <array>
#include <vector>

#include "../structs/enums.h"

//*******************************************
// partition class
//*******************************************

class Partition
{
public:
    std::vector<short> node_tiles;

    short get_node_tile(int node) { return this->node_tiles[node]; }

    short tile_count()
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
};
