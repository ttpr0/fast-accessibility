
#include "./loader.h"

//*******************************************
// util methods
//*******************************************

std::vector<char> readAllFile(std::string filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> arr(size);

    // allocate buffer
    char* buffer = arr.data();

    //read file
    if (!file.read(buffer, size)) {
        throw std::runtime_error("Failed to read file");
    }

    return arr;
}

std::vector<Node> loadGraphNodes(std::string file) {
    auto arr = readAllFile(file);
    char* buffer = arr.data();

    std::vector<Node> nodes;

    int nodecount = *(reinterpret_cast<int*>(&buffer[0]));
    for (int i = 0; i < nodecount; i++) {
        char type = *(reinterpret_cast<char*>(&buffer[4 + i]));
        nodes.push_back(Node{ type });
    }

    return nodes;
}

std::vector<Edge> loadGraphEdges(std::string file) {
    auto arr = readAllFile(file);
    auto buffer = &(*arr.begin());

    std::vector<Edge> edges;

    int edgecount = *(reinterpret_cast<int*>(&buffer[0]));
    for (int i = 0; i < edgecount; i++) {
        int node_a = *(reinterpret_cast<int*>(&buffer[4 + i * 14 + 0]));
        int node_b = *(reinterpret_cast<int*>(&buffer[4 + i * 14 + 4]));
        unsigned char type = *(reinterpret_cast<unsigned char*>(&buffer[4 + i * 14 + 8]));
        float length = *(reinterpret_cast<float*>(&buffer[4 + i * 14 + 9]));
        unsigned char maxspeed = *(reinterpret_cast<unsigned char*>(&buffer[4 + i * 14 + 13]));
        edges.push_back(Edge{ node_a, node_b, (RoadType)type, length, maxspeed, false });
    }

    return edges;
}

std::vector<Coord> loadGraphGeom(std::string file, int node_count, int edge_count) {
    auto arr = readAllFile(file);
    auto buffer = arr.data();

    std::vector<Coord> nodes;

    for (int i = 0; i < node_count; i++) {
        float lon = *(reinterpret_cast<float*>(&buffer[i * 8 + 0]));
        float lat = *(reinterpret_cast<float*>(&buffer[i * 8 + 4]));
        nodes.push_back(Coord{ lon, lat });
    }

    return nodes;
}

std::vector<int> loadEdgeWeights(std::string file, int edgecount) {
    auto arr = readAllFile(file);
    auto buffer = arr.data();

    std::vector<int> weights;

    for (int i = 0; i < edgecount; i++) {
        unsigned char weight = *(reinterpret_cast<unsigned char*>(&buffer[i * 1 + 0]));
        weights.push_back(weight);
    }

    return weights;
}

std::tuple<std::vector<_NodeEntry>, std::vector<_EdgeEntry>, std::vector<_EdgeEntry>> loadTopology(std::string file, int node_count) {
    auto arr = readAllFile(file);
    auto buffer = arr.data();

    std::vector<_NodeEntry> node_entries;
    std::vector<_EdgeEntry> fwd_edges;
    std::vector<_EdgeEntry> bwd_edges;

    int fwd_count = *(reinterpret_cast<int*>(&buffer[0]));
    int bwd_count = *(reinterpret_cast<int*>(&buffer[4]));

    int start = 8;
    for (int i = 0; i < node_count; i++) {
        int s1 = *(reinterpret_cast<int*>(&buffer[start + i * 12 + 0]));
        short c1 = *(reinterpret_cast<short*>(&buffer[start + i * 12 + 4]));
        int s2 = *(reinterpret_cast<int*>(&buffer[start + i * 12 + 6]));
        short c2 = *(reinterpret_cast<short*>(&buffer[start + i * 12 + 10]));
        node_entries.push_back(_NodeEntry{ s1, c1, s2, c2 });
    }
    start = 8 + node_count * 12;
    for (int i = 0; i < fwd_count; i++) {
        int edge_id = *(reinterpret_cast<int*>(&buffer[start + i * 8 + 0]));
        int other_id = *(reinterpret_cast<int*>(&buffer[start + i * 8 + 4]));
        fwd_edges.push_back(_EdgeEntry{ edge_id, other_id });
    }
    start = 8 + node_count * 12 + fwd_count * 8;
    for (int i = 0; i < bwd_count; i++) {
        int edge_id = *(reinterpret_cast<int*>(&buffer[start + i * 8 + 0]));
        int other_id = *(reinterpret_cast<int*>(&buffer[start + i * 8 + 4]));
        bwd_edges.push_back(_EdgeEntry{ edge_id, other_id });
    }

    return std::make_tuple(node_entries, fwd_edges, bwd_edges);
}

std::tuple<std::vector<_NodeEntry>, std::vector<_TypedEdgeEntry>, std::vector<_TypedEdgeEntry>> loadTypedTopology(std::string file, int node_count) {
    auto arr = readAllFile(file);
    auto buffer = arr.data();

    std::vector<_NodeEntry> node_entries;
    std::vector<_TypedEdgeEntry> fwd_edges;
    std::vector<_TypedEdgeEntry> bwd_edges;

    int fwd_count = *(reinterpret_cast<int*>(&buffer[0]));
    int bwd_count = *(reinterpret_cast<int*>(&buffer[4]));

    int start = 8;
    for (int i = 0; i < node_count; i++) {
        int s1 = *(reinterpret_cast<int*>(&buffer[start + i * 12 + 0]));
        short c1 = *(reinterpret_cast<short*>(&buffer[start + i * 12 + 4]));
        int s2 = *(reinterpret_cast<int*>(&buffer[start + i * 12 + 6]));
        short c2 = *(reinterpret_cast<short*>(&buffer[start + i * 12 + 10]));
        node_entries.push_back(_NodeEntry{ s1, c1, s2, c2 });
    }
    start = 8 + node_count * 12;
    for (int i = 0; i < fwd_count; i++) {
        int edge_id = *(reinterpret_cast<int*>(&buffer[start + i * 9 + 0]));
        unsigned char type = *(reinterpret_cast<unsigned char*>(&buffer[start + i * 9 + 4]));
        int other_id = *(reinterpret_cast<int*>(&buffer[start + i * 9 + 5]));
        fwd_edges.push_back(_TypedEdgeEntry{ edge_id, other_id, (char)type });
    }
    start = 8 + node_count * 12 + fwd_count * 9;
    for (int i = 0; i < bwd_count; i++) {
        int edge_id = *(reinterpret_cast<int*>(&buffer[start + i * 9 + 0]));
        unsigned char type = *(reinterpret_cast<unsigned char*>(&buffer[start + i * 9 + 4]));
        int other_id = *(reinterpret_cast<int*>(&buffer[start + i * 9 + 5]));
        bwd_edges.push_back(_TypedEdgeEntry{ edge_id, other_id, (char)type });
    }

    return std::make_tuple(node_entries, fwd_edges, bwd_edges);
}

std::tuple<std::vector<CHShortcut>, std::vector<int>> loadCHShortcuts(std::string file) {
    auto arr = readAllFile(file);
    auto buffer = arr.data();

    std::vector<CHShortcut> shortcuts;
    std::vector<int> weights;

    int shc_count = *(reinterpret_cast<int*>(&buffer[0]));

    int start = 4;
    for (int i = 0; i < shc_count; i++) {
        int node_a = *(reinterpret_cast<int*>(&buffer[start + i * 22 + 0]));
        int node_b = *(reinterpret_cast<int*>(&buffer[start + i * 22 + 4]));
        unsigned int weight = *(reinterpret_cast<unsigned int*>(&buffer[start + i * 22 + 8]));

        int e_id_1 = *(reinterpret_cast<int*>(&buffer[start + i * 22 + 0]));
        bool e_is_1 = *(reinterpret_cast<bool*>(&buffer[start + i * 22 + 4]));

        int e_id_2 = *(reinterpret_cast<int*>(&buffer[start + i * 22 + 0]));
        bool e_is_2 = *(reinterpret_cast<bool*>(&buffer[start + i * 22 + 4]));

        shortcuts.push_back(CHShortcut{ node_a, node_b, {std::make_tuple(e_id_1, e_is_1), std::make_tuple(e_id_2, e_is_2)} });
        weights.push_back(weight);
    }

    return std::make_tuple(shortcuts, weights);
}


std::vector<short> loadCHLevels(std::string file, int nodecount) {
    auto arr = readAllFile(file);
    auto buffer = arr.data();

    std::vector<short> levels;

    for (int i = 0; i < nodecount; i++) {
        short weight = *(reinterpret_cast<short*>(&buffer[i * 2 + 0]));
        levels.push_back(weight);
    }

    return levels;
}

//*******************************************
// load graphs
//*******************************************

std::unique_ptr<Graph> loadGraph(std::string path) {
    // load graph storage
    auto nodes = loadGraphNodes(path + "-nodes");
    int nodecount = nodes.size();
    auto edges = loadGraphEdges(path + "-edges");
    int edgecount = edges.size();
    auto node_geoms = loadGraphGeom(path + "-geom", nodecount, edgecount);

    // load topology
    auto temp2 = loadTopology(path + "-graph", nodecount);
    auto node_entries = std::get<0>(temp2);
    auto fwd_entries = std::get<1>(temp2);
    auto bwd_entries = std::get<2>(temp2);

    // load weights
    auto weights = loadEdgeWeights(path + "-fastest_weighting", edgecount);

    auto store = GraphStore(nodes, edges, node_geoms);
    auto topology = TopologyStore(node_entries, fwd_entries, bwd_entries);
    return std::make_unique<Graph>(store, topology, weights);
}

std::unique_ptr<CHGraph> loadCHGraph(std::string path) {
    // load graph storage
    auto nodes = loadGraphNodes(path + "-nodes");
    int nodecount = nodes.size();
    auto edges = loadGraphEdges(path + "-edges");
    int edgecount = edges.size();
    auto node_geoms = loadGraphGeom(path + "-geom", nodecount, edgecount);

    // load topology
    auto [node_entries, fwd_entries, bwd_entries] = loadTopology(path + "-graph", nodecount);

    // load weights
    auto weights = loadEdgeWeights(path + "-fastest_weighting", edgecount);

    // load ch-topology
    auto [ch_node_entries, ch_fwd_entries, ch_bwd_entries] = loadTopology(path + "-ch_graph", nodecount);

    // load ch store
    auto [shortcuts, sh_weights] = loadCHShortcuts(path + "-shortcut");
    auto node_levels = loadCHLevels(path + "-level", nodecount);

    auto store = GraphStore(nodes, edges, node_geoms);
    auto topology = TopologyStore(node_entries, fwd_entries, bwd_entries);
    auto ch_topology = TopologyStore(ch_node_entries, ch_fwd_entries, ch_bwd_entries);
    auto ch_store = CHStore(shortcuts, node_levels, sh_weights);
    return std::make_unique<CHGraph>(store, topology, weights, ch_store, ch_topology);
}
