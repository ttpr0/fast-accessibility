#include <vector>

#include "../../util/io.h"
#include "../base/graph_index.h"
#include "../base_graph.h"
#include "../ch_graph.h"
#include "../structs/adjacency.h"
#include "../structs/geom.h"
#include "../structs/structs.h"
#include "../tiled_graph.h"
#include "./graph_base_io.h"

std::shared_ptr<GraphBase> load_graph_base(const std::string& file)
{
    // load nodes
    auto arr = readAllFile(file + "-graph");
    auto reader = BufferReader(arr);
    int nodecount = reader.read<int>();
    std::vector<Node> nodes(nodecount);
    for (int i = 0; i < nodecount; i++) {
        float lon = reader.read<float>();
        float lat = reader.read<float>();
        nodes[i] = {.location = Coord{lon, lat}};
    }

    // load edges
    int edgecount = reader.read<int>();
    std::vector<Edge> edges(edgecount);
    for (int i = 0; i < edgecount; i++) {
        int node_a = reader.read<int>();
        int node_b = reader.read<int>();
        edges[i] = Edge{node_a, node_b};
    }

    AdjacencyArray adj = build_adjacency_array(nodes, edges);

    return std::make_shared<GraphBase>(std::move(nodes), std::move(edges), std::move(adj));
}

void store_graph_base(const GraphBase& base, const std::string& file)
{
    auto writer = BufferWriter();

    // store nodes
    writer.write<int>(base.nodes.size());
    for (int i = 0; i < base.nodes.size(); i++) {
        auto coord = base.nodes[i].location;
        writer.write<float>(coord.lon);
        writer.write<float>(coord.lat);
    }

    // store edges
    writer.write<int>(base.edges.size());
    for (int i = 0; i < base.edges.size(); i++) {
        auto edge = base.edges[i];
        writer.write<int>(edge.nodeA);
        writer.write<int>(edge.nodeB);
    }
    writeFile(file + "-graph", writer.bytes());
}
