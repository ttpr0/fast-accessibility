#pragma once

#include <string>

#include "../base_graph.h"
#include "../ch_graph.h"
#include "../comps/graph_index.h"
#include "../structs/adjacency.h"
#include "../structs/geom.h"
#include "../structs/structs.h"
#include "../tiled_graph.h"
#include "./adjaceny_io.h"
#include "./util.h"

std::shared_ptr<GraphBase> load_graph_base(const std::string& file)
{
    // load nodes
    auto arr = readAllFile(file + "-nodes");
    auto n_reader = BufferReader(arr);
    int nodecount = n_reader.read<int>();
    std::vector<Node> nodes(nodecount);
    for (int i = 0; i < nodecount; i++) {
        char type = n_reader.read<char>();
        nodes[i] = Node{type};
    }

    // load edges
    arr = readAllFile(file + "-edges");
    auto e_reader = BufferReader(arr);
    int edgecount = e_reader.read<int>();
    std::vector<Edge> edges(edgecount);
    for (int i = 0; i < edgecount; i++) {
        int node_a = e_reader.read<int>();
        int node_b = e_reader.read<int>();
        unsigned char type = e_reader.read<unsigned char>();
        float length = e_reader.read<float>();
        unsigned char maxspeed = e_reader.read<unsigned char>();
        edges[i] = Edge{node_a, node_b, (RoadType)type, length, maxspeed, false};
    }

    arr = readAllFile(file + "-geom");
    auto g_reader = BufferReader(arr);
    std::vector<Coord> node_geoms(nodecount);
    for (int i = 0; i < nodecount; i++) {
        float lon = g_reader.read<float>();
        float lat = g_reader.read<float>();
        node_geoms[i] = Coord{lon, lat};
    }

    AdjacencyArray adj = build_adjacency_array(nodes, edges);
    // KDTree tree = build_balanced_kdtree(node_geoms);
    KDTree tree = build_kdtree(node_geoms);

    return std::make_shared<GraphBase>(std::move(nodes), std::move(edges), std::move(node_geoms), std::move(adj), std::move(tree));
}

void store_graph_base(GraphBase& base, const std::string& file)
{
    // store nodes
    auto n_writer = BufferWriter();
    n_writer.write<int>(base.nodes.size());
    for (int i = 0; i < base.nodes.size(); i++) {
        n_writer.write<char>(base.nodes[i].type);
    }
    writeFile(file + "-nodes", n_writer.bytes());

    // store edges
    auto e_writer = BufferWriter();
    e_writer.write<int>(base.edges.size());
    for (int i = 0; i < base.edges.size(); i++) {
        auto edge = base.edges[i];
        e_writer.write<int>(edge.nodeA);
        e_writer.write<int>(edge.nodeB);
        e_writer.write<unsigned char>(edge.type);
        e_writer.write<float>(edge.length);
        e_writer.write<unsigned char>(edge.maxspeed);
    }
    writeFile(file + "-edges", e_writer.bytes());

    auto g_writer = BufferWriter();
    for (int i = 0; i < base.nodes.size(); i++) {
        auto coord = base.node_geoms[i];
        g_writer.write<float>(coord.lon);
        g_writer.write<float>(coord.lat);
    }
    writeFile(file + "-geom", g_writer.bytes());
}
