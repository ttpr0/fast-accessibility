#include <vector>

#include "../structs/adjacency.h"
#include "../structs/structs.h"
#include "./adjaceny_io.h"
#include "./id_mapping_io.h"
#include "./shortcut_io.h"
#include "./transit_data_io.h"
#include "./util.h"

std::shared_ptr<TransitData> load_transit_data(const std::string& file)
{
    auto arr = readAllFile(file + "-comps");
    auto reader = BufferReader(arr);

    auto stops = reader.read_vector<Node>();
    auto connections = reader.read_vector<Connection>();
    auto shortcuts = reader.read_vector<Shortcut>();

    auto adjacency = load_adjacency_array(file + "-adjacency", true);

    return std::make_shared<TransitData>(std::move(stops), std::move(connections), std::move(shortcuts), std::move(adjacency));
}

void store_transit_data(const TransitData& transit, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write_vector(std::span<const Node>{transit.stops.data(), transit.stops.size()});
    writer.write_vector(std::span<const Connection>{transit.connections.data(), transit.connections.size()});
    writer.write_vector(std::span<const Shortcut>{transit.shortcuts.data(), transit.shortcuts.size()});

    writeFile(file + "-comps", writer.bytes());
    store_adjacency_array(transit.adjacency, true, file + "-adjacency");
}
