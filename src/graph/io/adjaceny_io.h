#pragma once

#include <string>

#include "../structs/adjacency.h"
#include "./util.h"

static AdjacencyArray load_adjacency_array(const std::string& file, bool typed)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    std::vector<_NodeEntry> node_entries;
    std::vector<_EdgeEntry> fwd_edges;
    std::vector<_EdgeEntry> bwd_edges;

    int node_count = reader.read<int>();
    int fwd_count = reader.read<int>();
    int bwd_count = reader.read<int>();

    for (int i = 0; i < node_count; i++) {
        int s1 = reader.read<int>();
        short c1 = reader.read<short>();
        int s2 = reader.read<int>();
        short c2 = reader.read<short>();
        node_entries.push_back(_NodeEntry{s1, c1, s2, c2});
    }
    for (int i = 0; i < fwd_count; i++) {
        int edge_id = reader.read<int>();
        unsigned char type;
        if (typed) {
            type = reader.read<unsigned char>();
        }
        int other_id = reader.read<int>();
        fwd_edges.push_back(_EdgeEntry{edge_id, other_id, (char)type});
    }
    for (int i = 0; i < bwd_count; i++) {
        int edge_id = reader.read<int>();
        unsigned char type;
        if (typed) {
            type = reader.read<unsigned char>();
        }
        int other_id = reader.read<int>();
        bwd_edges.push_back(_EdgeEntry{edge_id, other_id, (char)type});
    }

    return {node_entries, fwd_edges, bwd_edges};
}

static void store_adjacency_array(const AdjacencyArray& adj, bool typed, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write<int>(adj.node_entries.size());
    writer.write<int>(adj.fwd_edge_entries.size());
    writer.write<int>(adj.bwd_edge_entries.size());

    for (int i = 0; i < adj.node_entries.size(); i++) {
        auto node_entry = adj.node_entries[i];
        writer.write<int>(node_entry.fwd_start);
        writer.write<short>(node_entry.fwd_count);
        writer.write<int>(node_entry.bwd_start);
        writer.write<short>(node_entry.bwd_count);
    }
    for (int i = 0; i < adj.fwd_edge_entries.size(); i++) {
        auto edge_entry = adj.fwd_edge_entries[i];
        writer.write<int>(edge_entry.edge_id);
        if (typed) {
            writer.write<unsigned char>(edge_entry.data[0]);
        }
        writer.write<int>(edge_entry.other_id);
    }
    for (int i = 0; i < adj.bwd_edge_entries.size(); i++) {
        auto edge_entry = adj.bwd_edge_entries[i];
        writer.write<int>(edge_entry.edge_id);
        if (typed) {
            writer.write<unsigned char>(edge_entry.data[0]);
        }
        writer.write<int>(edge_entry.other_id);
    }

    writeFile(file, writer.bytes());
}
