#include <vector>

#include "../../util/io.h"
#include "../structs/adjacency.h"
#include "../structs/structs.h"
#include "./id_mapping_io.h"
#include "./transit_data_io.h"

std::shared_ptr<TransitData> load_transit_data(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    auto stops = reader.read<std::vector<Node>>();
    auto connections = reader.read<std::vector<Connection>>();
    auto shortcuts = reader.read<std::vector<Shortcut>>();
    auto adjacency = reader.read<AdjacencyArray>();

    return std::make_shared<TransitData>(std::move(stops), std::move(connections), std::move(shortcuts), std::move(adjacency));
}

void store_transit_data(const TransitData& transit, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write(transit.stops);
    writer.write(transit.connections);
    writer.write(transit.shortcuts);
    writer.write(transit.adjacency);

    writeFile(file, writer.bytes());
}
