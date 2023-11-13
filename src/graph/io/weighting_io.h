#pragma once

#include <string>

#include "../comps/weighting.h"
#include "./util.h"

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

    return std::make_shared<Weighting>(weights);
}

void store_edge_weights(Weighting& weight, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write<int>(weight.edge_weights.size());
    for (int i = 0; i < weight.edge_weights.size(); i++) {
        writer.write<unsigned char>(weight.edge_weights[i]);
    }

    writeFile(file, writer.bytes());
}
