#include <vector>

#include "./transit_graph.h"

//*******************************************
// transit-graph
//*******************************************

TransitGraph build_transit_graph(std::shared_ptr<IGraph> base_graph, std::shared_ptr<TransitData> transit, std::shared_ptr<TransitWeighting> transit_weighting)
{
    return {base_graph, transit, transit_weighting};
}

IGraph& TransitGraph::getBaseGraph()
{
    return *this->base_graph;
}
DSnap TransitGraph::mapToBaseNode(int stop)
{
    return this->transit->mapToNode(stop);
}
void TransitGraph::mapToStops(int node, function_ref<void(Snap)> func)
{
    this->transit->mapToStops(node, func);
}

int TransitGraph::stopCount()
{
    return this->transit->stopCount();
}
int TransitGraph::connectionCount()
{
    return this->transit->connectionCount();
}
Node TransitGraph::getStop(int stop)
{
    return this->transit->getStop(stop);
}
Connection TransitGraph::getConnection(int conn)
{
    return this->transit->getConnection(conn);
}

void TransitGraph::forAdjacentConnections(int stop, Direction dir, Adjacency typ, function_ref<void(EdgeRef)> func)
{
    if (typ == ADJACENT_ALL) {
        auto accessor = this->transit->adjacency.getNeighbours(stop, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            std::array<char, 8> data = accessor.getData();
            func({edge_id, other_id, data[0]});
        }
    } else {
        throw 1;
    }
    return;
}
int TransitGraph::getShortcutWeight(EdgeRef shc)
{
    if (!shc.isShortcut()) {
        return 0;
    }
    auto shortcut = this->transit->getShortcut(shc.edge_id);
    return shortcut.weight;
}
std::optional<ConnectionWeight> TransitGraph::getConnectionWeight(EdgeRef conn, int from)
{
    if (conn.edge_id >= this->connectionCount() || conn.isShortcut()) {
        return std::nullopt;
    }
    return this->transit_weighting->get_next_weight(conn.edge_id, from);
}
std::span<const ConnectionWeight> TransitGraph::getConnectionWeight(EdgeRef conn, int from, int to)
{
    if (conn.edge_id >= this->connectionCount() || conn.isShortcut()) {
        return {};
    }
    return this->transit_weighting->get_weights_in_range(conn.edge_id, from, to);
}
int TransitGraph::getOtherStop(EdgeRef conn, int stop)
{
    if (conn.isShortcut()) {
        auto shc = this->transit->getShortcut(conn.edge_id);
        return shc.from == stop ? shc.to : shc.from;
    } else {
        auto con = this->transit->getConnection(conn.edge_id);
        return con.stopA == stop ? con.stopB : con.stopA;
    }
}
