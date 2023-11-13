#include <iostream>

#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unique_ptr.h>

#include "accessibility/dijkstra_e2sfca.h"
#include "accessibility/phast_e2sfca.h"
#include "accessibility/range_query.h"
#include "accessibility/tiled_e2sfca.h"
#include "graph/graph.h"
#include "graph/io/cell_index_io.h"
#include "graph/io/ch_data_io.h"
#include "graph/io/graph_base_io.h"
#include "graph/io/loader.h"
#include "graph/io/partition_io.h"
#include "graph/io/tiled_data_io.h"
#include "graph/io/weighting_io.h"
#include "graph/structs/geom.h"

namespace py = nanobind;
using namespace std;

NB_MODULE(_pyaccess_ext, m)
{
    //*******************************************
    // utility methods
    //*******************************************

    py::bind_vector<vector<int>>(m, "IntVector");
    py::bind_vector<vector<float>>(m, "FloatVector");
    auto coord_vec = py::bind_vector<vector<Coord>>(m, "CoordVector");
    coord_vec.def("append", [](vector<Coord>& v, tuple<float, float> val) {
        v.push_back(Coord{get<0>(val), get<1>(val)});
    });

    auto coord = py::class_<Coord>(m, "Coord");
    coord.def(py::init<>());
    coord.def_rw("lon", &Coord::lon);
    coord.def_rw("lat", &Coord::lat);

    //*******************************************
    // graph classes
    //*******************************************

    auto graph_base = py::class_<GraphBase>(m, "GraphBase");
    auto weighting = py::class_<Weighting>(m, "Weighting");
    auto partition = py::class_<Partition>(m, "Partition");
    auto ch_data = py::class_<CHData>(m, "CHData");
    auto ch_index = py::class_<_CHIndex>(m, "_CHIndex");
    auto ch_index_2 = py::class_<_CHIndex2>(m, "_CHIndex2");
    auto tiled_data = py::class_<TiledData>(m, "TiledData");
    auto cell_index = py::class_<_CellIndex>(m, "_CellIndex");

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

    //*******************************************
    // accessibility functions
    //*******************************************

    // 2sfca functions
    m.def("calc_dijkstra_2sfca", &calcDijkstra2SFCA);
    m.def("calc_range_phast_2sfca", &calcRangePHAST2SFCA);
    m.def("calc_range_rphast_2sfca", &calcRangeRPHAST2SFCA);
    m.def("calc_range_rphast_2sfca2", &calcRangeRPHAST2SFCA2);
    m.def("calc_tiled_2sfca", &calcTiled2SFCA);

    // range query functions
    m.def("calc_range_dijkstra", &calcDijkstraRangeQuery);
    m.def("calc_range_phast", &calcRangePHASTRangeQuery);
    m.def("calc_range_rphast", &calcRPHASTRangeQuery);
    m.def("calc_range_rphast_2", &calcRangeRPHASTRangeQuery);
    m.def("calc_gs_phast", &calcGSPHASTRangeQuery);
    m.def("calc_gs_rphast", &calcGSRPHASTRangeQuery);
    m.def("calc_grasp", &calcGRASPRangeQuery);
}
