#include <tuple>
#include <vector>

#include "./util.h"
#include "./weighting_io.h"

std::shared_ptr<Weighting> load_edge_weights(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    int edgecount = reader.read<int>();
    std::vector<int> weights(edgecount);
    for (int i = 0; i < edgecount; i++) {
        unsigned char weight = reader.read<unsigned char>();
        weights[i] = weight;
    }

    return std::make_shared<Weighting>(std::move(weights));
}

void store_edge_weights(const Weighting& weight, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write<int>(weight.edge_weights.size());
    for (int i = 0; i < weight.edge_weights.size(); i++) {
        writer.write<unsigned char>(weight.edge_weights[i]);
    }

    writeFile(file, writer.bytes());
}

std::shared_ptr<TCWeighting> load_tc_weights(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    int edge_count = reader.read<int>();
    int node_count = reader.read<int>();

    std::vector<int> edge_weights(edge_count);
    std::vector<std::tuple<char, char>> edge_indices(edge_count);
    for (int i = 0; i < edge_count; i++) {
        edge_weights[i] = reader.read<unsigned char>();
        edge_indices[i] = std::make_tuple(reader.read<char>(), reader.read<char>());
    }
    std::vector<std::tuple<int, char, char>> turn_refs(node_count);
    for (int i = 0; i < node_count; i++) {
        turn_refs[i] = std::make_tuple(reader.read<int>(), reader.read<char>(), reader.read<char>());
    }
    auto turn_weights = reader.read_vector<char>();

    return std::make_shared<TCWeighting>(std::move(edge_weights), std::move(edge_indices), std::move(turn_refs), std::move(turn_weights));
}

void store_tc_weights(const TCWeighting& weight, const std::string& file)
{
    auto writer = BufferWriter();

    int edge_count = weight.edge_weights.size();
    writer.write<int>(edge_count);
    int node_count = weight.turn_refs.size();
    writer.write<int>(node_count);

    for (int i = 0; i < edge_count; i++) {
        writer.write<unsigned char>(weight.edge_weights[i]);
        auto [ea, eb] = weight.edge_indices[i];
        writer.write<char>(ea);
        writer.write<char>(eb);
    }
    for (int i = 0; i < node_count; i++) {
        auto [loc, rows, cols] = weight.turn_refs[i];
        writer.write<int>(loc);
        writer.write<char>(rows);
        writer.write<char>(cols);
    }
    writer.write_vector<char>(weight.turn_weights);

    writeFile(file, writer.bytes());
}
