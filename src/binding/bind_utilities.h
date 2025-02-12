#pragma once

#include <iostream>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>

#include "../graph/structs/geom.h"
#include "../util/matrix.h"
#include "../util/osm/graph_parser.h"
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
    coord_vec.def("append", [](vector<Coord>& v, tuple<float, float> val) { v.push_back(Coord{get<0>(val), get<1>(val)}); }, "Insert Coordinate with (lon, lat).");
    auto node_vec = py::bind_vector<vector<Node>>(m, "NodeVector");
    node_vec.def("from_array", [](vector<Node>& v, VectorView<float> lons, VectorView<float> lats) {
        v.clear();
        v.reserve(lons.size());
        for (int i = 0; i < lons.size(); i++) {
            v.push_back({Coord{lons[i], lats[i]}});
        }
    });
    auto edge_vec = py::bind_vector<vector<Edge>>(m, "EdgeVector");
    edge_vec.def("from_array", [](vector<Edge>& v, VectorView<int> nodes_a, VectorView<int> nodes_b) {
        v.clear();
        v.reserve(nodes_a.size());
        for (int i = 0; i < nodes_a.size(); i++) {
            v.push_back({nodes_a[i], nodes_b[i]});
        }
    });
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

    m.def("graph_bounds", [](std::vector<Node>& nodes) {
        float minx = 1000000, maxx = -1000000, miny = 1000000, maxy = -1000000;
        for (int i = 0; i < nodes.size(); i++) {
            auto coord = nodes[i].location;
            minx = std::min(minx, coord.lon);
            maxx = std::max(maxx, coord.lon);
            miny = std::min(miny, coord.lat);
            maxy = std::max(maxy, coord.lat);
        }
        return std::make_tuple(minx, maxx, miny, maxy);
    });
    m.def("nodes_from_edges", [](std::vector<Edge>& edges) {
        Vector<int> node_a(edges.size());
        Vector<int> node_b(edges.size());
        for (int i = 0; i < edges.size(); i++) {
            node_a[i] = edges[i].nodeA;
            node_b[i] = edges[i].nodeB;
        }
        return std::make_tuple(node_a, node_b);
    });
    m.def("edges_with_nodes", [](std::vector<int>& nodes, VectorView<int> edge_node_a, VectorView<int> edge_node_b) {
        std::unordered_set<int> nodes_set(nodes.begin(), nodes.end());
        std::vector<int> edges;
        for (int i = 0; i < edge_node_a.size(); i++) {
            if (nodes_set.contains(edge_node_a[i]) || nodes_set.contains(edge_node_b[i])) {
                edges.push_back(i);
            }
        }
        return edges;
    });
    m.def("parse_graph_from_osm", &graph_from_osm);
}
