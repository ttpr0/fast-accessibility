#pragma once

#include <iostream>

#include <nanobind/nanobind.h>

#include "../graph/graph.h"
#include "../graph/io/cell_index_io.h"
#include "../graph/io/ch_data_io.h"
#include "../graph/io/graph_base_io.h"
#include "../graph/io/loader.h"
#include "../graph/io/partition_io.h"
#include "../graph/io/tiled_data_io.h"
#include "../graph/io/weighting_io.h"
#include "../graph/structs/geom.h"

void bind_graph(nanobind::module_& m)
{
    namespace py = nanobind;
    //*******************************************
    // graph classes
    //*******************************************

    auto graph_base = py::class_<GraphBase>(m, "GraphBase");
    auto weighting = py::class_<Weighting>(m, "Weighting");
    auto partition = py::class_<Partition>(m, "Partition");
    auto ch_data = py::class_<CHData>(m, "CHData");
    auto ch_index = py::class_<_CHIndex>(m, "CHIndex");
    auto ch_index_2 = py::class_<_CHIndex2>(m, "CHIndex2");
    auto tiled_data = py::class_<TiledData>(m, "TiledData");
    auto cell_index = py::class_<_CellIndex>(m, "CellIndex");

    auto i_graph = py::class_<IGraph>(m, "IGraph");
    auto i_ch_graph = py::class_<ICHGraph, IGraph>(m, "ICHGraph");
    auto i_tiled_graph = py::class_<ITiledGraph, IGraph>(m, "ITiledGraph");
    auto graph = py::class_<Graph, IGraph>(m, "Graph");
    auto ch_graph = py::class_<CHGraph, ICHGraph>(m, "CHGraph");
    auto ch_graph_2 = py::class_<CHGraph2, IGraph>(m, "CHGraph2");
    auto tiled_graph = py::class_<TiledGraph, ITiledGraph>(m, "TiledGraph");

    //*******************************************
    // graph utility methods
    //*******************************************

    m.def("load_graph_base", &load_graph_base);
    m.def("load_edge_weights", &load_edge_weights);
    m.def("load_node_partition", &load_node_partition);
    m.def("load_ch_data", &load_ch_data);
    m.def("load_tiled_data", &load_tiled_data);
    m.def("load_cell_index", &load_cell_index);

    m.def("build_base_graph", &build_base_graph);
    m.def("build_tiled_graph", &build_tiled_graph);
    m.def("build_ch_graph", &build_ch_graph);
    m.def("build_ch_graph_2", &build_ch_graph_2);

    m.def("build_ch_index", &build_ch_index);
    m.def("build_ch_index_2", &build_ch_index_2);

    m.def("load_graph", &loadGraph);
    m.def("load_chgraph", &loadCHGraph);
    m.def("load_chgraph_2", &loadCHGraph2);
    m.def("load_tiled_graph", &loadTiledGraph);
}
