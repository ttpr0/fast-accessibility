#include <iostream>

#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unique_ptr.h>

#include "accessibility/dijkstra_e2sfca.h"
#include "accessibility/phast_e2sfca.h"
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
    coord.def("lon", [](Coord& c) { return c.lon; });
    coord.def("lat", [](Coord& c) { return c.lat; });

    auto i_graph = py::class_<IGraph>(m, "IGraph");
    auto i_ch_graph = py::class_<ICHGraph, IGraph>(m, "ICHGraph");
    auto graph = py::class_<Graph, IGraph>(m, "Graph");
    auto ch_graph = py::class_<CHGraph, ICHGraph>(m, "CHGraph");

    m.def("load_graph", &loadGraph);
    m.def("load_chgraph", &loadCHGraph);

    m.def("calc_dijkstra_2sfca", &calcDijkstra2SFCA);
}