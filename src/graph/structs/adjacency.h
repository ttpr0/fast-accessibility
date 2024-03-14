#pragma once

#include <array>
#include <vector>

#include "../../util/io.h"
#include "./enums.h"
#include "./structs.h"
#include "./table.h"

//*******************************************
// adjacency structs
//*******************************************

struct _EdgeEntry
{
    int edge_id;
    int other_id;
    std::array<char, 8> data;
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
    StaticTable<_EdgeEntry> fwd_entries;
    StaticTable<_EdgeEntry> bwd_entries;

    AdjacencyArray(StaticTable<_EdgeEntry> fwd_entries, StaticTable<_EdgeEntry> bwd_entries) : fwd_entries(fwd_entries), bwd_entries(bwd_entries) {}

    // return the node degree for given direction
    short getDegree(int node, Direction dir) const;
    AdjacencyAccessor getNeighbours(int node, Direction dir) const;
};

class AdjacencyList
{
public:
    DynamicTable<_EdgeEntry> fwd_entries;
    DynamicTable<_EdgeEntry> bwd_entries;

    AdjacencyList(int node_count)
    {
        this->fwd_entries = DynamicTable<_EdgeEntry>(node_count);
        this->bwd_entries = DynamicTable<_EdgeEntry>(node_count);
    }

    // return the node degree for given direction
    short getDegree(int node, Direction dir) const;
    AdjacencyAccessor getNeighbours(int node, Direction dir) const;

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
    TableAccessor<_EdgeEntry> state;

    AdjacencyAccessor(TableAccessor<_EdgeEntry> state) : state(state) {}

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

//*******************************************
// read and write adjacency
//*******************************************

template <>
struct ReadWrite<AdjacencyArray>
{
    AdjacencyArray read(BufferReader& reader)
    {
        auto fwd_entries = reader.read<StaticTable<_EdgeEntry>>();
        auto bwd_entries = reader.read<StaticTable<_EdgeEntry>>();

        return {fwd_entries, bwd_entries};
    }
    void write(BufferWriter& writer, const AdjacencyArray& adj)
    {
        writer.write(adj.fwd_entries);
        writer.write(adj.bwd_entries);
    }
};
