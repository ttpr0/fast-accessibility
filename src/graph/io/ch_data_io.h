#pragma once

#include <string>

#include "../speed_ups/ch_data.h"
#include "../structs/adjacency.h"
#include "../structs/structs.h"
#include "./adjaceny_io.h"
#include "./id_mapping_io.h"
#include "./shortcut_io.h"
#include "./util.h"

std::shared_ptr<CHData> load_ch_data(const std::string& file)
{
    auto node_levels = read_vector_from_file<short>(file + "-level");
    auto shortcuts = load_shortcuts(file + "-shortcut");
    auto adjacency = load_adjacency_array(file + "-ch_graph", false, node_levels.size());

    return std::make_shared<CHData>(std::move(shortcuts), std::move(adjacency), std::move(node_levels));
}

void store_ch_data(CHData& ch, const std::string& file)
{
    write_vector_to_file<short>(ch.node_levels, file + "-level");
    store_shortcuts(ch.shortcuts, file + "-shortcut");
    store_adjacency_array(ch.topology, false, file + "-ch_graph");
}
