#pragma once

#include <vector>

#include "../../../solver/one_to_many/dijkstra.h"
#include "../../base/graph_base.h"
#include "../../base/id_mapping.h"
#include "../../base_graph.h"
#include "../../speed_ups/transit_data.h"
#include "../../structs/adjacency.h"
#include "../../weights/weighting.h"

static std::vector<DSnap> map_stops_to_graph(IGraphIndex& index, const std::vector<Node>& stops)
{
    std::vector<Coord> stop_coords(stops.size());
    for (int i = 0; i < stops.size(); i++) {
        stop_coords[i] = stops[i].location;
    }
    return index.mapToClosest(stop_coords);
}

static std::shared_ptr<TransitData> build_transit_data(const GraphBase& base, const Weighting& weight, std::vector<Node>& stops, std::vector<DSnap>& stop_mapping,
                                                       std::vector<Connection>& connections, int max_range)
{
    auto graph = BaseGraphExplorer(base, weight);
    // build id-mapping
    auto mapping = NodeMapping(graph.nodeCount(), stops.size(), stop_mapping);

    // build stop-connections
    auto adjacency_list = AdjacencyList(stops.size());
    for (int i = 0; i < connections.size(); i++) {
        auto connection = connections[i];
        adjacency_list.addEdgeEntries(connection.stopA, connection.stopB, i, 30);
    }
    auto shortcuts = std::vector<Shortcut>();
    auto solver = RangeDijkstra(graph);
    solver.addMaxRange(max_range);
    solver.build();
    auto state = solver.makeComputeState();
    for (int i = 0; i < stops.size(); i++) {
        auto s_snap = mapping.get_source(i);
        if (s_snap.len() == 0) {
            continue;
        }
        solver.compute(s_snap, state);
        for (int j = 0; j < stops.size(); j++) {
            if (i == j) {
                continue;
            }
            auto t_snap = mapping.get_source(j);
            if (t_snap.len() == 0) {
                continue;
            }
            int dist = state.getDistance(t_snap);
            if (dist > max_range) {
                continue;
            }
            int shc_id = shortcuts.size();
            shortcuts.push_back(Shortcut{.from = i, .to = j, .weight = dist});
            adjacency_list.addEdgeEntries(i, j, shc_id, 120);
        }
    }
    auto adjacency = build_adjacency_array(adjacency_list);

    return std::make_shared<TransitData>(mapping, stops, connections, shortcuts, adjacency);
}

static std::shared_ptr<TransitData> build_transit_data_tc(const GraphBase& base, const TCWeighting& weight, std::vector<Node>& stops, std::vector<DSnap>& stop_mapping,
                                                       std::vector<Connection>& connections, int max_range)
{
    auto graph = TCGraphExplorer(base, weight);
    // build id-mapping
    auto mapping = NodeMapping(graph.nodeCount(), stops.size(), stop_mapping);

    // build stop-connections
    auto adjacency_list = AdjacencyList(stops.size());
    for (int i = 0; i < connections.size(); i++) {
        auto connection = connections[i];
        adjacency_list.addEdgeEntries(connection.stopA, connection.stopB, i, 30);
    }
    auto shortcuts = std::vector<Shortcut>();
    auto solver = RangeDijkstraTC(graph);
    solver.addMaxRange(max_range);
    solver.build();
    auto state = solver.makeComputeState();
    for (int i = 0; i < stops.size(); i++) {
        auto s_snap = mapping.get_source(i);
        if (s_snap.len() == 0) {
            continue;
        }
        solver.compute(s_snap, state);
        for (int j = 0; j < stops.size(); j++) {
            if (i == j) {
                continue;
            }
            auto t_snap = mapping.get_source(j);
            if (t_snap.len() == 0) {
                continue;
            }
            int dist = state.getDistance(t_snap);
            if (dist > max_range) {
                continue;
            }
            int shc_id = shortcuts.size();
            shortcuts.push_back(Shortcut{.from = i, .to = j, .weight = dist});
            adjacency_list.addEdgeEntries(i, j, shc_id, 120);
        }
    }
    auto adjacency = build_adjacency_array(adjacency_list);

    return std::make_shared<TransitData>(mapping, stops, connections, shortcuts, adjacency);
}
