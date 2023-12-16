#pragma once

#include <string>

#include "../speed_ups/partition.h"
#include "./util.h"

std::shared_ptr<Partition> load_node_partition(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    int nodecount = reader.read<int>();
    std::vector<short> tiles(nodecount);
    for (int i = 0; i < nodecount; i++) {
        short tile = reader.read<short>();
        tiles[i] = tile;
    }

    return std::make_shared<Partition>(tiles);
}

void store_node_partition(Partition& partition, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write<int>(partition.node_tiles.size());
    for (int i = 0; i < partition.node_tiles.size(); i++) {
        writer.write<short>(partition.node_tiles[i]);
    }

    writeFile(file, writer.bytes());
}
