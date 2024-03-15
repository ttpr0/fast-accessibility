#pragma once

#include <array>
#include <unordered_set>
#include <vector>

#include "../../util/function_ref.h"
#include "../structs/adjacency.h"
#include "../structs/node_mapping.h"
#include "../structs/structs.h"

//*******************************************
// transit class
//*******************************************

class TransitData
{
public:
    NodeMapping mapping;
    std::vector<Node> stops;
    std::vector<Connection> connections;
    std::vector<Shortcut> shortcuts;
    AdjacencyArray adjacency;

    int stopCount() const { return this->stops.size(); }
    Node getStop(int stop) const { return this->stops[stop]; }
    bool hasStops(int node) const { return this->mapping.has_targets(node); }
    void mapToStops(int node, function_ref<void(Snap)> callback) const { this->mapping.for_targets(node, callback); }
    DSnap mapToNode(int stop) const { return this->mapping.get_source(stop); }
    int connectionCount() const { return this->connections.size(); }
    Connection getConnection(int connection) const { return this->connections[connection]; }
    int shortcutCount() const { return this->shortcuts.size(); }
    Shortcut getShortcut(int shortcut) const { return this->shortcuts[shortcut]; }
};
