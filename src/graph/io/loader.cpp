#define _FILE_OFFSET_BITS 64
#define _LARGE_FILE_SOURCE 1

#include <cstring>
#include <iterator>

#include "../preproc/build_ch_index.h"
#include "./cell_index_io.h"
#include "./ch_data_io.h"
#include "./graph_base_io.h"
#include "./id_mapping_io.h"
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
    auto index = build_balanced_kdtree_index(*base);
    auto weight = load_edge_weights(path + "");

    // build graph
    return build_base_graph(std::move(base), std::move(weight), std::move(index));
}

CHGraph loadCHGraph(const std::string& path)
{
    // load components
    auto base = load_graph_base(path + "");
    auto index = build_balanced_kdtree_index(*base);
    auto weight = load_edge_weights(path + "");
    auto id_mapping = load_id_mapping(path + "_mapping");
    auto ch = load_ch_data(path + "");
    auto ch_index = build_ch_index(*base, *weight, *ch, *id_mapping);

    // build graph
    return build_ch_graph(std::move(base), std::move(weight), std::move(index), std::move(id_mapping), std::move(ch), std::move(ch_index));
}

TiledGraph loadTiledGraph(const std::string& path)
{
    // load components
    auto base = load_graph_base(path + "");
    auto index = build_balanced_kdtree_index(*base);
    auto weight = load_edge_weights(path + "");
    auto partition = load_node_partition(path + "");
    auto id_mapping = load_id_mapping(path + "_mapping");
    auto tiled = load_tiled_data(path + "", base->nodeCount());
    auto cell_index = load_cell_index(path + "");

    // build graph
    return build_tiled_graph(std::move(base), std::move(weight), std::move(index), std::move(partition), std::move(id_mapping), std::move(tiled), std::move(cell_index));
}

CHGraph2 loadCHGraph2(const std::string& path)
{
    // load components
    auto base = load_graph_base(path + "");
    auto index = build_balanced_kdtree_index(*base);
    auto weight = load_edge_weights(path + "");
    auto ch = load_ch_data(path + "");
    auto partition = load_node_partition(path + "");
    auto id_mapping = load_id_mapping(path + "_mapping");
    auto ch_index = build_ch_index_2(*base, *weight, *ch, *partition, *id_mapping);

    // build graph
    return build_ch_graph_2(std::move(base), std::move(weight), std::move(index), std::move(partition), std::move(id_mapping), std::move(ch), std::move(ch_index));
}