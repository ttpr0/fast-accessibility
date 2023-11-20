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
    // graph structs
    //*******************************************
    auto road_type = py::enum_<RoadType>(m, "RoadType", "Type of road.");
    road_type.value("MOTORWAY", RoadType::MOTORWAY);
    road_type.value("MOTORWAY_LINK", RoadType::MOTORWAY_LINK);
    road_type.value("TRUNK", RoadType::TRUNK);
    road_type.value("TRUNK_LINK", RoadType::TRUNK_LINK);
    road_type.value("PRIMARY", RoadType::PRIMARY);
    road_type.value("PRIMARY_LINK", RoadType::PRIMARY_LINK);
    road_type.value("SECONDARY", RoadType::SECONDARY);
    road_type.value("SECONDARY_LINK", RoadType::SECONDARY_LINK);
    road_type.value("TERTIARY", RoadType::TERTIARY);
    road_type.value("TERTIARY_LINK", RoadType::TERTIARY_LINK);
    road_type.value("RESIDENTIAL", RoadType::RESIDENTIAL);
    road_type.value("LIVING_STREET", RoadType::LIVING_STREET);
    road_type.value("UNCLASSIFIED", RoadType::UNCLASSIFIED);
    road_type.value("ROAD", RoadType::ROAD);
    road_type.value("TRACK", RoadType::TRACK);

    auto edge = py::class_<Edge>(m, "Edge");
    edge.def(py::init<>());
    edge.def("__init__", [](Edge& e, int nodeA, int nodeB, RoadType typ, float length, int maxspeed) {
        e.nodeA = nodeA;
        e.nodeB = nodeB;
        e.type = typ;
        e.length = length;
        e.maxspeed = maxspeed;
    });
    edge.def_rw("node_a", &Edge::nodeA);
    edge.def_rw("node_b", &Edge::nodeB);
    edge.def_rw("typ", &Edge::type);
    edge.def_rw("length", &Edge::length);
    edge.def_rw("maxspeed", &Edge::maxspeed);

    auto node = py::class_<Node>(m, "Node");
    node.def(py::init<>());
    node.def_rw("typ", &Node::type);

    auto shortcut = py::class_<Shortcut>(m, "Shortcut");
    shortcut.def(py::init<>());
    shortcut.def("__init__", [](Shortcut& s, int from, int to, int weight) {
        s.from = from;
        s.to = to;
        s.weight = weight;
    });
    shortcut.def_rw("from_", &Shortcut::from);
    shortcut.def_rw("to_", &Shortcut::to);
    shortcut.def_rw("weight", &Shortcut::weight);

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

    m.def("store_graph_base", &store_graph_base);
    m.def("store_edge_weights", &store_edge_weights);
    m.def("store_node_partition", &store_node_partition);
    m.def("store_ch_data", &store_ch_data);
    m.def("store_tiled_data", &store_tiled_data);
    m.def("store_cell_index", &store_cell_index);

    m.def("build_ch_index", &build_ch_index);
    m.def("build_ch_index_2", &build_ch_index_2);
    m.def("build_graph_base", &build_graph_base);
    m.def("build_default_weighting", &build_default_weighting);

    m.def("build_base_graph", &build_base_graph);
    m.def("build_tiled_graph", &build_tiled_graph);
    m.def("build_ch_graph", &build_ch_graph);
    m.def("build_ch_graph_2", &build_ch_graph_2);
}
