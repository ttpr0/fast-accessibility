#include <vector>

#include "../../util/io.h"
#include "../structs/adjacency.h"
#include "../structs/structs.h"
#include "./cell_index_io.h"

std::shared_ptr<_CellIndex> load_cell_index(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    int fwd_tile_count = reader.read<int>();
    int bwd_tile_count = reader.read<int>();

    std::unordered_map<short, std::vector<Shortcut>> fwd_index_edges;
    for (int i = 0; i < fwd_tile_count; i++) {
        short tile = reader.read<short>();
        int count = reader.read<int>();
        std::vector<Shortcut> edges(count);
        for (int j = 0; j < count; j++) {
            int from = reader.read<int>();
            int to = reader.read<int>();
            int weight = reader.read<int>();
            auto payload = reader.read<Data<4>>();
            edges[j] = {from, to, weight, payload};
        }
        fwd_index_edges[tile] = edges;
    }
    std::unordered_map<short, std::vector<Shortcut>> bwd_index_edges;
    for (int i = 0; i < bwd_tile_count; i++) {
        short tile = reader.read<short>();
        int count = reader.read<int>();
        std::vector<Shortcut> edges(count);
        for (int j = 0; j < count; j++) {
            int from = reader.read<int>();
            int to = reader.read<int>();
            int weight = reader.read<int>();
            auto payload = reader.read<Data<4>>();
            edges[j] = {from, to, weight, payload};
        }
        bwd_index_edges[tile] = edges;
    }

    return std::make_shared<_CellIndex>(fwd_index_edges, bwd_index_edges);
}

void store_cell_index(const _CellIndex& index, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write<int>(index.fwd_index_edges.size());
    writer.write<int>(index.bwd_index_edges.size());

    for (auto& [tile, edges] : index.fwd_index_edges) {
        writer.write<short>(tile);
        writer.write<int>(edges.size());
        for (int j = 0; j < edges.size(); j++) {
            auto shc = edges[j];
            writer.write<int>(shc.from);
            writer.write<int>(shc.to);
            writer.write<int>(shc.weight);
            writer.write<Data<4>>(shc.payload);
        }
    }
    for (auto& [tile, edges] : index.bwd_index_edges) {
        writer.write<short>(tile);
        writer.write<int>(edges.size());
        for (int j = 0; j < edges.size(); j++) {
            auto shc = edges[j];
            writer.write<int>(shc.from);
            writer.write<int>(shc.to);
            writer.write<int>(shc.weight);
            writer.write<Data<4>>(shc.payload);
        }
    }

    writeFile(file, writer.bytes());
}
