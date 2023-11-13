#pragma once

#include <array>
#include <vector>

#include "./enums.h"
#include "./structs.h"

//*******************************************
// adjacency structs
//*******************************************

struct _NodeEntry
{
    int fwd_start;
    short fwd_count;
    int bwd_start;
    short bwd_count;
};

struct _EdgeEntry
{
    int edge_id;
    int other_id;
    std::array<char, 8> data;
};

struct _DynamicNodeEntry
{
    std::vector<_EdgeEntry> fwd_edges;
    std::vector<_EdgeEntry> bwd_edges;
};

//*******************************************
// adjacency classes
//*******************************************

class AdjacencyArray;
class AdjacencyList;
class AdjacencyAccessor;

class AdjacencyArray
{
public:
    std::vector<_NodeEntry> node_entries;
    std::vector<_EdgeEntry> fwd_edge_entries;
    std::vector<_EdgeEntry> bwd_edge_entries;

    AdjacencyArray(std::vector<_NodeEntry> node_entries, std::vector<_EdgeEntry> fwd_entries,
                   std::vector<_EdgeEntry> bwd_entries)
        : node_entries(node_entries), fwd_edge_entries(fwd_entries), bwd_edge_entries(bwd_entries)
    {}

    // return the node degree for given direction
    short getDegree(int node, Direction dir);
    AdjacencyAccessor getNeighbours(int node, Direction dir);
};

class AdjacencyList
{
public:
    std::vector<_DynamicNodeEntry> node_entries;

    AdjacencyList(int node_count)
    {
        std::vector<_DynamicNodeEntry> topology(node_count);

        for (int i = 0; i < node_count; i++) {
            topology[i] = _DynamicNodeEntry{std::vector<_EdgeEntry>(), std::vector<_EdgeEntry>()};
        }

        this->node_entries = topology;
    }

    // return the node degree for given direction
    short getDegree(int node, Direction dir);
    AdjacencyAccessor getNeighbours(int node, Direction dir);

    void addEdgeEntries(int node_a, int node_b, int edge_id, char edge_typ);

    // adds forward entry to adjacency
    //
    // refers to edge between node_a and node_b, entry will be added at node_a
    void addFWDEntry(int node_a, int node_b, int edge_id, char edge_typ);

    // adds backward entry to adjacency
    //
    // refers to edge between node_a and node_b, entry will be added at node_b
    void addBWDEntry(int node_a, int node_b, int edge_id, char edge_typ);
};

//*******************************************
// adjacency accessor
//*******************************************

class AdjacencyAccessor
{
public:
    int state;
    int end;
    _EdgeEntry* edge_refs;

    AdjacencyAccessor(_EdgeEntry* edge_refs, int state, int end) : state(state), end(end), edge_refs(edge_refs) {}

    bool next();
    int getEdgeID();
    int getOtherID();
    std::array<char, 8> getData();
};

//*******************************************
// build adjacency
//*******************************************

AdjacencyArray build_adjacency_array(std::vector<Node>& nodes, std::vector<Edge>& edges);
AdjacencyArray build_adjacency_array(AdjacencyList& adj_list);
