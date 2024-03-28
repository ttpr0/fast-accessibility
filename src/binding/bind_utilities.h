#pragma once

#include <iostream>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>

#include "../graph/structs/geom.h"
#include "../util/snap.h"

NB_MAKE_OPAQUE(std::vector<int>);
NB_MAKE_OPAQUE(std::vector<float>);
NB_MAKE_OPAQUE(std::vector<Coord>);
NB_MAKE_OPAQUE(std::vector<Node>);
NB_MAKE_OPAQUE(std::vector<Edge>);
NB_MAKE_OPAQUE(std::vector<Connection>);
NB_MAKE_OPAQUE(std::vector<DSnap>);

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
    py::bind_vector<vector<Connection>>(m, "ConnectionVector");
    py::bind_vector<vector<DSnap>>(m, "SnapVector");

    auto coord = py::class_<Coord>(m, "Coord");
    coord.def(py::init<>());
    coord.def(py::init<float, float>());
    coord.def_rw("lon", &Coord::lon);
    coord.def_rw("lat", &Coord::lat);

    auto snap = py::class_<Snap>(m, "Snap");
    snap.def(py::init<>());
    snap.def(py::init<int, int>());
    snap.def_rw("node", &Snap::node);
    snap.def_rw("dist", &Snap::dist);

    auto dsnap = py::class_<DSnap>(m, "DSnap");
    dsnap.def(py::init<>());
    dsnap.def(py::init<int>());
    dsnap.def(py::init<Snap>());
    dsnap.def(py::init<Snap, Snap>());
    dsnap.def("len", &DSnap::len);
    dsnap.def("__getitem__", [](DSnap& snap, int index) { return snap[index]; });
    dsnap.def("__setitem__", [](DSnap& snap, int index, Snap item) { snap[index] = item; });
}
