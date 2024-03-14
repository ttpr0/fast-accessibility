#include <vector>

#include "../../util/io.h"
#include "../structs/adjacency.h"
#include "../structs/structs.h"
#include "./ch_data_io.h"
#include "./id_mapping_io.h"

std::shared_ptr<CHData> load_ch_data(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    auto node_levels = reader.read<std::vector<short>>();
    auto shortcuts = reader.read<std::vector<Shortcut>>();
    auto adjacency = reader.read<AdjacencyArray>();

    return std::make_shared<CHData>(std::move(shortcuts), std::move(adjacency), std::move(node_levels));
}

void store_ch_data(const CHData& ch, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write(ch.node_levels);
    writer.write(ch.shortcuts);
    writer.write(ch.topology);

    writeFile(file, writer.bytes());
}
