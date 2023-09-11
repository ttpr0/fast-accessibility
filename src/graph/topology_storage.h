#pragma once

#include <vector>

#include "./enums.h"

//*******************************************
// topology structs
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
};

struct _TypedEdgeEntry
{
    int edge_id;
    int other_id;
    char type;
};

struct _DynamicNodeEntry
{
    std::vector<_TypedEdgeEntry> fwd_edges;
    std::vector<_TypedEdgeEntry> bwd_edges;
};

//*******************************************
// topology classes
//*******************************************

class TopologyStore;
class TypedTopologyStore;
class DynamicTopologyStore;

class TopologyAccessor;
class TypedTopologyAccessor;
class DynamicTopologyAccessor;

//*******************************************
// topology store
//*******************************************

class TopologyStore
{
public:
    std::vector<_NodeEntry> node_entries;
    std::vector<_EdgeEntry> fwd_edge_entries;
    std::vector<_EdgeEntry> bwd_edge_entries;

    TopologyStore(std::vector<_NodeEntry> node_entries, std::vector<_EdgeEntry> fwd_entries, std::vector<_EdgeEntry> bwd_entries)
        : node_entries(node_entries), fwd_edge_entries(fwd_entries), bwd_edge_entries(bwd_entries)
    {}

    // return the node degree for given direction
    short getDegree(int node, Direction dir);
    TopologyAccessor getAccessor();
};

class TypedTopologyStore
{
public:
    std::vector<_NodeEntry> node_entries;
    std::vector<_TypedEdgeEntry> fwd_edge_entries;
    std::vector<_TypedEdgeEntry> bwd_edge_entries;

    TypedTopologyStore(std::vector<_NodeEntry> node_entries, std::vector<_TypedEdgeEntry> fwd_entries, std::vector<_TypedEdgeEntry> bwd_entries)
        : node_entries(node_entries), fwd_edge_entries(fwd_entries), bwd_edge_entries(bwd_entries)
    {}

    // return the node degree for given direction
    short getDegree(int node, Direction dir);
    TypedTopologyAccessor getAccessor();
};

class DynamicTopologyStore
{
public:
    std::vector<_DynamicNodeEntry> node_entries;

    DynamicTopologyStore(int node_count)
    {
        std::vector<_DynamicNodeEntry> topology(node_count);

        for (int i = 0; i < node_count; i++) {
            topology[i] = _DynamicNodeEntry{ std::vector<_TypedEdgeEntry>(), std::vector<_TypedEdgeEntry>() };
        }

        this->node_entries = topology;
    }

    // return the node degree for given direction
    short getDegree(int node, Direction dir);

    void addEdgeEntries(int node_a, int node_b, int edge_id, char edge_typ);

    // adds forward entry to adjacency
    //
    // refers to edge between node_a and node_b, entry will be added at node_a
    void addFWDEntry(int node_a, int node_b, int edge_id, char edge_typ);

    // adds backward entry to adjacency
    //
    // refers to edge between node_a and node_b, entry will be added at node_b
    void addBWDEntry(int node_a, int node_b, int edge_id, char edge_typ);

    DynamicTopologyAccessor getAccessor();
};

//*******************************************
// topology accessor
//*******************************************

class TopologyAccessor
{
public:
    TopologyStore* topology;
    int state;
    int end;
    _EdgeEntry* edge_refs;
    int curr_edge_id;
    int curr_other_id;

    TopologyAccessor(TopologyStore* topology) {
        this->topology = topology;
    }

    void setBaseNode(int node, Direction dir);
    bool next();
    int getEdgeID();
    int getOtherID();
};

class TypedTopologyAccessor
{
public:
    TypedTopologyStore* topology;
    int state;
    int end;
    _TypedEdgeEntry* edge_refs;
    int curr_edge_id;
    int curr_other_id;
    char curr_type;

    TypedTopologyAccessor(TypedTopologyStore* topology) {
        this->topology = topology;
    }

    void setBaseNode(int node, Direction dir);
    bool next();
    int getEdgeID();
    int getOtherID();
    char getType();
};

class DynamicTopologyAccessor
{
public:
    DynamicTopologyStore* topology;
    int state;
    int end;
    _TypedEdgeEntry* edge_refs;
    int curr_edge_id;
    int curr_other_id;
    char curr_type;

    DynamicTopologyAccessor(DynamicTopologyStore* topology) {
        this->topology = topology;
    }

    void SetBaseNode(int node, Direction dir);
    bool next();
    int getEdgeID();
    int getOtherID();
    char getType();
};
