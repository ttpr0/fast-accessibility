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
}
