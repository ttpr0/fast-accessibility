#pragma once

#include <array>
#include <unordered_set>
#include <vector>

#include "../structs/adjacency.h"
#include "../structs/structs.h"

//*******************************************
// transit class
//*******************************************

class TransitData
{
public:
    std::vector<Node> stops;
    std::vector<Connection> connections;
    std::vector<Shortcut> shortcuts;
    AdjacencyArray adjacency;

    int stopCount() const { return this->stops.size(); }
    Node getStop(int stop) const { return this->stops[stop]; }
    int connectionCount() const { return this->connections.size(); }
    Connection getConnection(int connection) const { return this->connections[connection]; }
    int shortcutCount() const { return this->shortcuts.size(); }
    Shortcut getShortcut(int shortcut) const { return this->shortcuts[shortcut]; }
};
