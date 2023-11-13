#define _FILE_OFFSET_BITS 64
#define _LARGE_FILE_SOURCE 1

#include <cstring>
#include <iterator>

#include "./cell_index_io.h"
#include "./ch_data_io.h"
#include "./graph_base_io.h"
#include "./loader.h"
#include "./partition_io.h"
#include "./tiled_data_io.h"
#include "./util.h"
#include "./weighting_io.h"

//*******************************************
// load graphs
//*******************************************

Graph loadGraph(const std::string& path)
{
    // load components
    auto base = load_graph_base(path + "");
    auto weight = load_edge_weights(path + "");

    // build graph
    return build_base_graph(std::move(base), std::move(weight));
}

CHGraph loadCHGraph(const std::string& path)
{
    // load components
    auto base = load_graph_base(path + "");
    auto weight = load_edge_weights(path + "");
    auto ch = load_ch_data(path + "");
    auto ch_index = build_ch_index(*base, *weight, *ch);

    // build graph
    return build_ch_graph(std::move(base), std::move(weight), std::move(ch), std::move(ch_index));
}

TiledGraph loadTiledGraph(const std::string& path)
{
    // load components
    auto base = load_graph_base(path + "");
    auto weight = load_edge_weights(path + "");
    auto tiled = load_tiled_data(path + "", base->nodeCount());
    auto partition = load_node_partition(path + "");
    auto cell_index = load_cell_index(path + "");

    // build graph
    return build_tiled_graph(std::move(base), std::move(weight), std::move(partition), std::move(tiled),
                             std::move(cell_index));
}

CHGraph2 loadCHGraph2(const std::string& path)
{
    // load components
    auto base = load_graph_base(path + "");
    auto weight = load_edge_weights(path + "");
    auto ch = load_ch_data(path + "");
    auto partition = load_node_partition(path + "");
    auto ch_index = build_ch_index_2(*base, *weight, *ch, *partition);

    // build graph
    return build_ch_graph_2(std::move(base), std::move(weight), std::move(partition), std::move(ch),
                            std::move(ch_index));
}