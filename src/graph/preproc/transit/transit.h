#pragma once

#include <vector>

#include "../../../solver/one_to_many/dijkstra.h"
#include "../../base/graph_base.h"
#include "../../base/id_mapping.h"
#include "../../base_graph.h"
#include "../../speed_ups/transit_data.h"
#include "../../structs/adjacency.h"
#include "../../weights/weighting.h"

static std::shared_ptr<TransitData> build_transit_data(IGraph& graph, IGraphIndex& index, std::vector<Node> stops, std::vector<Connection> connections, int max_range)
{
    // build id-mapping
    std::vector<Coord> stop_coords(stops.size());
    for (int i = 0; i < stops.size(); i++) {
        stop_coords[i] = stops[i].location;
    }
    auto mapping = NodeMapping(graph.nodeCount(), stops.size(), index.mapToClosest(stop_coords));

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
