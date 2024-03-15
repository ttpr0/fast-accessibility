#pragma once

#include <math.h>
#include <memory>
#include <vector>

#include "../util/function_ref.h"
#include "./base/graph_base.h"
#include "./base/graph_index.h"
#include "./base/id_mapping.h"
#include "./base_graph.h"
#include "./graph.h"
#include "./speed_ups/transit_data.h"
#include "./structs/adjacency.h"
#include "./weights/transit_weighting.h"

//*******************************************
// transit-graph
//*******************************************

class TransitGraph
{
public:
    std::shared_ptr<IGraph> base_graph;

    // additional components
    std::shared_ptr<TransitData> transit;
    std::shared_ptr<TransitWeighting> transit_weighting;

    TransitGraph(std::shared_ptr<IGraph> base_graph, std::shared_ptr<TransitData> transit, std::shared_ptr<TransitWeighting> transit_weighting)
        : base_graph(base_graph), transit(transit), transit_weighting(transit_weighting)
    {}

    IGraph& getBaseGraph();
    DSnap mapToBaseNode(int stop);
    void mapToStops(int node, function_ref<void(Snap)> func);

    int stopCount();
    int connectionCount();
    Node getStop(int stop);
    Connection getConnection(int conn);

    void forAdjacentConnections(int stop, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func);
    int getShortcutWeight(EdgeRef shc);
    std::optional<ConnectionWeight> getConnectionWeight(EdgeRef conn, int from);
    std::span<const ConnectionWeight> getConnectionWeight(EdgeRef conn, int from, int to);
    int getOtherStop(EdgeRef conn, int stop);
};

//*******************************************
// build transit-graph
//*******************************************

TransitGraph build_transit_graph(std::shared_ptr<IGraph> base_graph, std::shared_ptr<TransitData> transit, std::shared_ptr<TransitWeighting> transit_weighting);
