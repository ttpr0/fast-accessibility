#pragma once

#include <iostream>
#include <memory>

#include <nanobind/nanobind.h>

#include "../graph/base/graph_index.h"
#include "../graph/ch_graph.h"
#include "../graph/graph.h"
#include "../graph/io/cell_index_io.h"
#include "../graph/io/ch_data_io.h"
#include "../graph/io/graph_base_io.h"
#include "../graph/io/id_mapping_io.h"
#include "../graph/io/partition_io.h"
#include "../graph/io/tiled_data_io.h"
#include "../graph/io/transit_data_io.h"
#include "../graph/io/weighting_io.h"
#include "../graph/mod/ch_mod.h"
#include "../graph/mod/graph_base_mod.h"
#include "../graph/mod/id_mapping_mod.h"
#include "../graph/mod/overlay_mod.h"
#include "../graph/mod/partition_mod.h"
#include "../graph/mod/weighting_mod.h"
#include "../graph/preproc/build_ch_index.h"
#include "../graph/preproc/ch/ch.h"
#include "../graph/preproc/node_ordering.h"
#include "../graph/preproc/partition/partition.h"
#include "../graph/preproc/remove_unconnected.h"
#include "../graph/preproc/tiled/tiled.h"
#include "../graph/preproc/transit/transit.h"
#include "../graph/structs/geom.h"
#include "../graph/tiled_graph.h"
#include "../graph/transit_graph.h"
#include "../graph/weights/tc_weighting.h"
#include "../graph/weights/transit_weighting.h"
#include "../graph/weights/weighting.h"

void bind_graph(nanobind::module_& m)
{
    namespace py = nanobind;
    //*******************************************
    // graph structs
    //*******************************************
    // auto road_type = py::enum_<RoadType>(m, "RoadType", "Type of road.");
    // road_type.value("MOTORWAY", RoadType::MOTORWAY);
    // road_type.value("MOTORWAY_LINK", RoadType::MOTORWAY_LINK);
    // road_type.value("TRUNK", RoadType::TRUNK);
    // road_type.value("TRUNK_LINK", RoadType::TRUNK_LINK);
    // road_type.value("PRIMARY", RoadType::PRIMARY);
    // road_type.value("PRIMARY_LINK", RoadType::PRIMARY_LINK);
    // road_type.value("SECONDARY", RoadType::SECONDARY);
    // road_type.value("SECONDARY_LINK", RoadType::SECONDARY_LINK);
    // road_type.value("TERTIARY", RoadType::TERTIARY);
    // road_type.value("TERTIARY_LINK", RoadType::TERTIARY_LINK);
    // road_type.value("RESIDENTIAL", RoadType::RESIDENTIAL);
    // road_type.value("LIVING_STREET", RoadType::LIVING_STREET);
    // road_type.value("UNCLASSIFIED", RoadType::UNCLASSIFIED);
    // road_type.value("ROAD", RoadType::ROAD);
    // road_type.value("TRACK", RoadType::TRACK);

    auto direction = py::enum_<Direction>(m, "Direction", "Incoming or outgoing edges.");
    direction.value("FORWARD", Direction::FORWARD);
    direction.value("BACKWARD", Direction::BACKWARD);
    direction.export_values();

    auto edge = py::class_<Edge>(m, "Edge");
    edge.def(py::init<>());
    edge.def("__init__", [](Edge& e, int nodeA, int nodeB) {
        e.nodeA = nodeA;
        e.nodeB = nodeB;
    });
    edge.def_rw("node_a", &Edge::nodeA);
    edge.def_rw("node_b", &Edge::nodeB);

    auto node = py::class_<Node>(m, "Node");
    node.def(py::init<>());
    node.def("__init__", [](Node& n, Coord loc) { n.location = loc; });
    node.def_rw("loc", &Node::location);

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

    auto connection = py::class_<Connection>(m, "Connection");
    connection.def(py::init<>());
    connection.def("__init__", [](Connection& c, int stop_a, int stop_b, int route) {
        c.stopA = stop_a;
        c.stopB = stop_b;
        c.route = route;
    });
    connection.def_rw("stop_a", &Connection::stopA);
    connection.def_rw("stop_b", &Connection::stopB);
    connection.def_rw("route", &Connection::route);

    //*******************************************
    // graph base components
    //*******************************************

    auto graph_base = py::class_<GraphBase>(m, "GraphBase");
    graph_base.def("get_node", &GraphBase::getNode);
    graph_base.def("get_node_geom", &GraphBase::getNodeGeom);
    graph_base.def("node_count", &GraphBase::nodeCount);
    graph_base.def("get_edge", &GraphBase::getEdge);
    graph_base.def("edge_count", &GraphBase::edgeCount);
    graph_base.def("get_adjacent_edges", [](GraphBase& base, int node, Direction dir) {
        py::list l;
        auto accessor = base.adjacency.getNeighbours(node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            l.append(py::int_(edge_id));
        }
        return l;
    });

    auto weighting = py::class_<Weighting>(m, "Weighting");
    weighting.def("get_edge_weight", &Weighting::get_edge_weight);
    weighting.def("set_edge_weight", &Weighting::set_edge_weight);
    weighting.def("set_edge_weights", [](Weighting& w, const VectorView<float> weights) {
        if (weights.size() != w.edge_weights.size()) {
            throw std::runtime_error("weights.size() != w.edge_weights.size()");
        }
        for (int i = 0; i < weights.size(); i++) {
            w.set_edge_weight(i, weights(i));
        }
    });

    auto tc_weighting = py::class_<TCWeighting>(m, "TCWeighting");
    tc_weighting.def("get_edge_weight", &TCWeighting::get_edge_weight);
    tc_weighting.def("set_edge_weight", &TCWeighting::set_edge_weight);
    tc_weighting.def("set_edge_weights", [](TCWeighting& w, const VectorView<float> weights) {
        if (weights.size() != w.edge_weights.size()) {
            throw std::runtime_error("weights.size() != w.edge_weights.size()");
        }
        for (int i = 0; i < weights.size(); i++) {
            w.set_edge_weight(i, weights(i));
        }
    });
    tc_weighting.def("get_turn_cost", &TCWeighting::get_turn_cost);
    tc_weighting.def("set_turn_cost", &TCWeighting::set_turn_cost);

    auto i_graph_index = py::class_<IGraphIndex>(m, "IGraphIndex");
    i_graph_index.def("get_closest_node", static_cast<DSnap (IGraphIndex::*)(Coord)>(&IGraphIndex::getClosestNode));
    i_graph_index.def("get_closest_node", static_cast<DSnap (IGraphIndex::*)(float, float)>(&IGraphIndex::getClosestNode));
    i_graph_index.def("get_closest_node", static_cast<DSnap (IGraphIndex::*)(Coord, const _IDMapping&)>(&IGraphIndex::getClosestNode));
    i_graph_index.def("get_closest_node", static_cast<DSnap (IGraphIndex::*)(float, float, const _IDMapping&)>(&IGraphIndex::getClosestNode));
    i_graph_index.def("map_to_closest", static_cast<std::vector<DSnap> (IGraphIndex::*)(const std::vector<Coord>&)>(&IGraphIndex::mapToClosest));
    i_graph_index.def("map_to_closest", static_cast<std::vector<DSnap> (IGraphIndex::*)(VectorView<float>, VectorView<float>)>(&IGraphIndex::mapToClosest));
    i_graph_index.def("map_to_closest", static_cast<std::vector<DSnap> (IGraphIndex::*)(const std::vector<Coord>&, const _IDMapping&)>(&IGraphIndex::mapToClosest));
    i_graph_index.def("map_to_closest", static_cast<std::vector<DSnap> (IGraphIndex::*)(VectorView<float>, VectorView<float>, const _IDMapping&)>(&IGraphIndex::mapToClosest));
    i_graph_index.def("map_to_closest", [](IGraphIndex& index, const std::vector<Node>& stops) { return map_stops_to_graph(index, stops); });

    auto id_mapping = py::class_<_IDMapping>(m, "IDMapping");
    id_mapping.def("get_source", &_IDMapping::get_source);
    id_mapping.def("get_target", &_IDMapping::get_target);

    //*******************************************
    // graph speed-up components
    //*******************************************

    auto partition = py::class_<Partition>(m, "Partition");
    partition.def("get_node_tile", &Partition::get_node_tile);

    auto ch_data = py::class_<CHData>(m, "CHData");
    ch_data.def("get_shortcut", &CHData::getShortcut);
    ch_data.def("shortcut_count", &CHData::shortcutCount);
    ch_data.def("get_node_level", &CHData::getNodeLevel);
    ch_data.def("get_adjacent_shortcuts", [](CHData& ch, int node, Direction dir) {
        py::list l;
        auto accessor = ch.topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int shc_id = accessor.getEdgeID();
            l.append(py::int_(shc_id));
        }
        return l;
    });

    auto ch_index = py::class_<_CHIndex>(m, "CHIndex");

    auto tiled_data = py::class_<TiledData>(m, "TiledData");
    tiled_data.def("get_shortcut", &TiledData::getShortcut);
    tiled_data.def("shortcut_count", &TiledData::shortcutCount);
    tiled_data.def("get_edge_type", [](TiledData& tiled, int edge) { return (int)tiled.getEdgeType(edge); });
    tiled_data.def("get_adjacent_shortcuts", [](TiledData& tiled, int node, Direction dir) {
        py::list l;
        auto accessor = tiled.topology.getNeighbours(node, dir);
        while (accessor.next()) {
            int shc_id = accessor.getEdgeID();
            std::array<char, 8> data = accessor.getData();
            if (data[0] < 100) {
                continue;
            }
            l.append(py::int_(shc_id));
        }
        return l;
    });

    auto cell_index = py::class_<_CellIndex>(m, "CellIndex");

    auto transit_data = py::class_<TransitData>(m, "TransitData");
    transit_data.def("stop_count", &TransitData::stopCount);
    transit_data.def("connection_count", &TransitData::connectionCount);
    transit_data.def("get_stop", &TransitData::getStop);
    transit_data.def("has_node_stops", &TransitData::hasStops);
    transit_data.def("map_node_to_stops", [](TransitData& transit, int node) {
        py::list l;
        transit.mapping.for_targets(node, [&](Snap snap) { l.append(snap); });
        return l;
    });
    transit_data.def("map_stop_to_node", &TransitData::mapToNode);
    transit_data.def("get_connection", &TransitData::getConnection);
    transit_data.def("get_adjacent_connections", [](TransitData& transit, int stop, Direction dir) {
        py::list l;
        auto accessor = transit.adjacency.getNeighbours(stop, dir);
        while (accessor.next()) {
            int conn_id = accessor.getEdgeID();
            std::array<char, 8> data = accessor.getData();
            if (data[0] > 100) {
                continue;
            }
            l.append(py::int_(conn_id));
        }
        return l;
    });

    auto transit_weighting = py::class_<TransitWeighting>(m, "TransitWeighting");
    transit_weighting.def("get_connection_weight", [](TransitWeighting& tw, int connection, int from) -> std::optional<std::tuple<int, int>> {
        auto w = tw.get_next_weight(connection, from);
        if (w) {
            return std::make_tuple(w->departure, w->arrival);
        } else {
            return std::nullopt;
        }
    });
    transit_weighting.def("set_connection_schedule", [](TransitWeighting& tw, int connection, std::vector<std::tuple<int, int>> schedule) {
        std::vector<ConnectionWeight> weights;
        for (auto& s : schedule) {
            weights.push_back(ConnectionWeight(std::get<0>(s), std::get<1>(s), 0));
        }
        tw.set_weights(connection, weights);
    });

    //*******************************************
    // graph classes
    //*******************************************

    auto i_graph = py::class_<IGraph>(m, "IGraph");
    auto i_ch_graph = py::class_<ICHGraph, IGraph>(m, "ICHGraph");
    auto i_tiled_graph = py::class_<ITiledGraph, IGraph>(m, "ITiledGraph");
    auto graph = py::class_<Graph, IGraph>(m, "Graph");
    auto tc_graph = py::class_<TCGraph, IGraph>(m, "TCGraph");
    auto ch_graph = py::class_<CHGraph, ICHGraph>(m, "CHGraph");
    auto ch_graph_2 = py::class_<CHGraph2, IGraph>(m, "CHGraph2");
    auto tiled_graph = py::class_<TiledGraph, ITiledGraph>(m, "TiledGraph");
    auto transit_graph = py::class_<TransitGraph>(m, "TransitGraph");

    //*******************************************
    // graph utility methods
    //*******************************************

    m.def("load_graph_base", &load_graph_base);
    m.def("load_edge_weights", &load_edge_weights);
    m.def("load_tc_weights", &load_tc_weights);
    m.def("load_transit_weights", &load_transit_weights);
    m.def("load_node_partition", &load_node_partition);
    m.def("load_ch_data", &load_ch_data);
    m.def("load_tiled_data", &load_tiled_data);
    m.def("load_cell_index", &load_cell_index);
    m.def("load_id_mapping", &load_id_mapping);
    m.def("load_transit_data", &load_transit_data);

    m.def("store_graph_base", &store_graph_base);
    m.def("store_edge_weights", &store_edge_weights);
    m.def("store_tc_weights", &store_tc_weights);
    m.def("store_transit_weights", &store_transit_weights);
    m.def("store_node_partition", &store_node_partition);
    m.def("store_ch_data", &store_ch_data);
    m.def("store_tiled_data", &store_tiled_data);
    m.def("store_cell_index", &store_cell_index);
    m.def("store_id_mapping", &store_id_mapping);
    m.def("store_transit_data", &store_transit_data);

    m.def("prepare_ch_simple", &calc_simple_contraction);
    m.def("prepare_ch_phast", &calc_phast_contraction);
    m.def("prepare_ch_tiled", &calc_tiled_contraction);
    m.def("prepare_ch_index", &build_ch_index);
    m.def("prepare_ch_index_2", &build_ch_index_2);
    m.def("new_graph_base", &new_graph_base);
    m.def("new_id_mapping", &new_id_mapping);
    m.def("new_weighting", &build_weighting);
    m.def("calc_crossing_penalties", &turn_cost_penalty);
    m.def("new_tc_weighting", &build_tc_weighting);
    m.def("new_transit_weighting", &build_transit_weighting);
    m.def("prepare_kdtree_index", &build_kdtree_index);
    m.def("prepare_balanced_kdtree_index", &build_balanced_kdtree_index);
    m.def("prepare_partition", &calc_partition);
    m.def("prepare_tiled", &calc_clique_overlay);
    m.def("prepare_cell_index", &calc_full_cell_index);
    m.def("prepare_isophast", &calc_isophast_overlay);
    m.def("prepare_transit", &build_transit_data);
    m.def("prepare_transit", &build_transit_data_tc);

    m.def("build_base_graph", &build_base_graph);
    m.def("build_tc_graph", &build_tc_graph);
    m.def("build_tiled_graph", &build_tiled_graph);
    m.def("build_ch_graph", &build_ch_graph);
    m.def("build_ch_graph_2", &build_ch_graph_2);
    m.def("build_transit_graph", &build_transit_graph);

    m.def("calc_unconnected", &extract_unconnected);
    m.def("remove_nodes", &_remove_nodes);
    m.def("calc_dfs_order", &build_base_order);
    m.def("calc_ch_order", &build_ch_order);
    m.def("calc_ch2_order", &build_ch2_order);
    m.def("calc_mapped_order", &build_mapped_mapping);
    m.def("calc_identity_order", build_identity_mapping);
    m.def("reorder_nodes", static_cast<std::shared_ptr<GraphBase> (*)(const GraphBase&, const std::vector<int>&)>(&_reorder_nodes));
    m.def("reorder_nodes", static_cast<std::shared_ptr<Weighting> (*)(const Weighting&, const std::vector<int>&)>(&_reorder_nodes));
    m.def("reorder_nodes", static_cast<std::shared_ptr<TCWeighting> (*)(const TCWeighting&, const std::vector<int>&)>(&_reorder_nodes));
    m.def("reorder_nodes", static_cast<std::shared_ptr<Partition> (*)(const Partition&, const std::vector<int>&)>(&_reorder_nodes));
    m.def("reorder_nodes", static_cast<std::shared_ptr<CHData> (*)(const CHData&, const std::vector<int>&)>(&_reorder_nodes));
    m.def("reorder_nodes", static_cast<std::shared_ptr<TiledData> (*)(const TiledData&, const std::vector<int>&)>(&_reorder_nodes));
    m.def("reorder_nodes", static_cast<std::shared_ptr<_CHIndex> (*)(const _CHIndex&, const std::vector<int>&)>(&_reorder_nodes));
    m.def("reorder_nodes", static_cast<std::shared_ptr<_CellIndex> (*)(const _CellIndex&, const std::vector<int>&)>(&_reorder_nodes));
    m.def("reorder_sources", &_reorder_sources);
    m.def("reorder_targets", &_reorder_targets);
}
