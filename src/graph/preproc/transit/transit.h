#pragma once

#include <vector>

#include "../../../solver/one_to_many/dijkstra.h"
#include "../../base/graph_base.h"
#include "../../base/id_mapping.h"
#include "../../base_graph.h"
#include "../../speed_ups/transit_data.h"
#include "../../structs/adjacency.h"
#include "../../weights/weighting.h"

static std::tuple<std::shared_ptr<TransitData>, std::shared_ptr<_IDMapping>> build_transit_data(IGraph& graph, std::vector<Node> stops, std::vector<Connection> connections,
                                                                                                int max_range)
{
    // build id-mapping
    auto mapping = std::vector<std::array<int, 2>>(graph.nodeCount());
    for (int i = 0; i < graph.nodeCount(); i++) {
        mapping[i] = {-1, -1};
    }
    for (int i = 0; i < stops.size(); i++) {
        auto stop = stops[i];
        int c_id = graph.getClosestNode(stop.location);
        if (c_id == -1) {
            continue;
        }
        mapping[c_id][0] = i;
        mapping[i][1] = c_id;
    }
    auto id_mapping = std::make_shared<_IDMapping>(mapping);

    // build stop-connections
    auto adjacency_list = AdjacencyList(stops.size());
    for (int i = 0; i < connections.size(); i++) {
        auto connection = connections[i];
        adjacency_list.addEdgeEntries(connection.stopA, connection.stopB, i, 30);
    }
    auto shortcuts = std::vector<Shortcut>();
    auto solver = RangeDijkstraTC(&graph);
    solver.addMaxRange(max_range);
    solver.build();
    auto state = solver.makeComputeState();
    for (int i = 0; i < stops.size(); i++) {
        int s_node = id_mapping->get_source(i);
        if (s_node == -1) {
            continue;
        }
        solver.compute(s_node, state);
        for (int j = 0; j < stops.size(); j++) {
            if (i == j) {
                continue;
            }
            int t_node = id_mapping->get_source(j);
            if (t_node == -1) {
                continue;
            }
            int dist = state.getDistance(t_node);
            if (dist > max_range) {
                continue;
            }
            int shc_id = shortcuts.size();
            shortcuts.push_back(Shortcut{.from = i, .to = j, .weight = dist});
            adjacency_list.addEdgeEntries(i, j, shc_id, 120);
        }
    }
    auto adjacency = build_adjacency_array(adjacency_list);

    return std::make_tuple(std::make_shared<TransitData>(stops, connections, shortcuts, adjacency), id_mapping);
}
