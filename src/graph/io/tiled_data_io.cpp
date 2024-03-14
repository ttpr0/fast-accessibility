#include <vector>

#include "../../util/io.h"
#include "../structs/adjacency.h"
#include "../structs/structs.h"
#include "./ch_data_io.h"
#include "./id_mapping_io.h"
#include "./tiled_data_io.h"

std::shared_ptr<TiledData> load_tiled_data(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    auto shortcuts = reader.read<std::vector<Shortcut>>();
    auto edge_types = reader.read<std::vector<char>>();
    auto adjacency = reader.read<AdjacencyArray>();

    return std::make_shared<TiledData>(std::move(shortcuts), std::move(adjacency), std::move(edge_types));
}

void store_tiled_data(const TiledData& tiled, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write(tiled.shortcuts);
    writer.write(tiled.edge_types);
    writer.write(tiled.topology);

    writeFile(file, writer.bytes());
}
