#include <iostream>

#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unique_ptr.h>

#include "accessibility/dijkstra_e2sfca.h"
#include "accessibility/other_e2sfca.h"
#include "accessibility/phast_e2sfca.h"
#include "accessibility/range_query.h"
#include "accessibility/tiled_e2sfca.h"
#include "graph/geom.h"
#include "graph/graph.h"
#include "graph/loader.h"

namespace py = nanobind;
using namespace std;

NB_MODULE(_pyaccess_ext, m)
{
    py::bind_vector<vector<int>>(m, "IntVector");
    py::bind_vector<vector<float>>(m, "FloatVector");
    auto coord_vec = py::bind_vector<vector<Coord>>(m, "CoordVector");
    coord_vec.def("append", [](vector<Coord>& v, tuple<float, float> val) { v.push_back(Coord{get<0>(val), get<1>(val)}); });

    auto coord = py::class_<Coord>(m, "Coord");
    coord.def(py::init<>());
    coord.def_rw("lon", &Coord::lon);
    coord.def_rw("lat", &Coord::lat);

    auto i_graph = py::class_<IGraph>(m, "IGraph");
    auto i_ch_graph = py::class_<ICHGraph, IGraph>(m, "ICHGraph");
    auto i_tiled_graph = py::class_<ITiledGraph, IGraph>(m, "ITiledGraph");
    auto graph = py::class_<Graph, IGraph>(m, "Graph");
    auto ch_graph = py::class_<CHGraph, ICHGraph>(m, "CHGraph");
    auto ch_graph_2 = py::class_<CHGraph2, ICHGraph>(m, "CHGraph2");
    auto tiled_graph = py::class_<TiledGraph, ITiledGraph>(m, "TiledGraph");

    m.def("load_graph", &loadGraph);
    m.def("load_chgraph", &loadCHGraph);
    m.def("load_chgraph_2", &loadCHGraph2);
    m.def("load_tiled_graph", &loadTiledGraph);

    // 2sfca functions
    m.def("calc_dijkstra_2sfca", &calcDijkstra2SFCA);
    m.def("calc_dijkstra_2sfca_2", &_calcDijkstra2SFCA);
    m.def("calc_range_phast_2sfca", &calcRangePHAST2SFCA);
    m.def("calc_range_rphast_2sfca", &calcRangeRPHAST2SFCA);
    m.def("calc_range_rphast_2sfca2", &calcRangeRPHAST2SFCA2);
    m.def("calc_range_rphast_2sfca3", &calcRangeRPHAST2SFCA3);
    m.def("calc_tiled_2sfca", &calcTiled2SFCA);
    m.def("calc_tiled_2sfca2", &_calcGRASP2SFCA);

    // range query functions
    m.def("calc_range_dijkstra", &calcDijkstraRangeQuery);
    m.def("calc_range_phast", &calcRangePHASTRangeQuery);
    m.def("calc_range_rphast", &calcRPHASTRangeQuery);
    m.def("calc_range_rphast_2", &calcRangeRPHASTRangeQuery);
    m.def("calc_gs_phast", &calcGSPHASTRangeQuery);
    m.def("calc_gs_rphast", &calcGSRPHASTRangeQuery);
    m.def("calc_grasp", &calcGRASPRangeQuery);
}