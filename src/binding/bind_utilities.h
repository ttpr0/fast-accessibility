#pragma once

#include <iostream>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unique_ptr.h>

#include "../graph/structs/geom.h"
#include "../graph/util/map_coords.h"

void bind_utilities(nanobind::module_& m)
{
    namespace py = nanobind;
    using namespace std;

    //*******************************************
    // utility methods
    //*******************************************

    py::bind_vector<vector<int>>(m, "IntVector");
    py::bind_vector<vector<float>>(m, "FloatVector");
    auto coord_vec = py::bind_vector<vector<Coord>>(m, "CoordVector");
    coord_vec.def(
        "append",
        [](vector<Coord>& v, tuple<float, float> val) {
            v.push_back(Coord{get<0>(val), get<1>(val)});
        },
        "Insert Coordinate with (lon, lat).");
    py::bind_vector<vector<Node>>(m, "NodeVector");
    py::bind_vector<vector<Edge>>(m, "EdgeVector");

    auto coord = py::class_<Coord>(m, "Coord");
    coord.def(py::init<>());
    coord.def(py::init<float, float>());
    coord.def_rw("lon", &Coord::lon);
    coord.def_rw("lat", &Coord::lat);

    m.def("map_to_closest", static_cast<std::vector<int> (*)(const std::vector<Coord>&, IGraph&)>(&map_to_closest));
    m.def("map_to_closest", static_cast<int (*)(Coord, IGraph&)>(&map_to_closest));
    m.def("map_to_closest", [](py::list& l, IGraph& graph) {
        std::vector<int> closest(l.size());
        for (int i = 0; i < l.size(); i++) {
            py::tuple t = py::cast<py::tuple>(l[i]);
            py::handle lon = t[0];
            py::handle lat = t[1];
            Coord c = {py::cast<float>(lon), py::cast<float>(lat)};
            closest[i] = graph.getClosestNode(c);
        }
        return closest;
    });
    m.def("map_to_closest", [](py::tuple& t, IGraph& graph) {
        py::handle lon = t[0];
        py::handle lat = t[1];
        Coord c = {py::cast<float>(lon), py::cast<float>(lat)};
        return graph.getClosestNode(c);
    });
}
