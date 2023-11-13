#pragma once

#include <string>

#include "../comps/tiled_data.h"
#include "../structs/adjacency.h"
#include "../structs/structs.h"
#include "./adjaceny_io.h"
#include "./ch_data_io.h"
#include "./id_mapping_io.h"
#include "./shortcut_io.h"
#include "./util.h"

std::shared_ptr<TiledData> load_tiled_data(const std::string& file, int nodecount)
{
    auto id_mapping = load_id_mapping(file + "-mapping");
    auto shortcuts = load_shortcuts(file + "-skip_shortcuts");
    auto edge_types = read_vector_from_file<char>(file + "-tiles_types");
    auto adjacency = load_adjacency_array(file + "-skip_topology", true, nodecount);

    return std::make_shared<TiledData>(std::move(id_mapping), std::move(shortcuts), std::move(adjacency),
                                       std::move(edge_types));
}

void store_tiled_data(TiledData& tiled, const std::string& file, int nodecount)
{
    store_id_mapping(tiled.id_mapping, file + "-mapping");
    store_shortcuts(tiled.shortcuts, file + "-skip_shortcuts");
    write_vector_to_file<char>(tiled.edge_types, file + "-tiles_types");
    store_adjacency_array(tiled.topology, true, file + "-skip_topology");
}
