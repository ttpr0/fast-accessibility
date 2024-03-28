#pragma once

#include <array>
#include <memory>
#include <vector>

#include "../../util/function_ref.h"
#include "../../util/io.h"
#include "../../util/snap.h"
#include "./table.h"

//*******************************************
// node-mapping
//*******************************************

// Maps indices from source-nodes to target-stops and reversed.
class NodeMapping
{
public:
    StaticTable<Snap> source_mapping;
    std::vector<DSnap> target_mapping;

    NodeMapping(StaticTable<Snap> source_mapping, std::vector<DSnap> target_mapping) : source_mapping(source_mapping), target_mapping(target_mapping) {}

    // Builds a new node-mapping
    // source_count -> number of nodes in the graph
    // target_count -> number of stops mapped into the graph
    // mapping -> actual stop to node mapping
    NodeMapping(int source_count, int target_count, const std::vector<DSnap>& mapping)
    {
        DynamicTable<Snap> table(source_count);
        for (int i = 0; i < target_count; i++) {
            auto dsnap = mapping[i];
            for (int j = 0; j < dsnap.len(); j++) {
                auto snap = dsnap[j];
                table.addValue(i, {snap.node, snap.dist});
            }
        }
        this->target_mapping = mapping;
        this->source_mapping = table.toStatic();
    }

    bool has_targets(int source) const { return this->source_mapping.getCount(source) > 0; }

    // iterates over all target stops mapped to a source node
    void for_targets(int source, function_ref<void(Snap)> callback) const
    {
        auto accessor = this->source_mapping.getValues(source);
        while (accessor.next()) {
            callback(accessor.getValue());
        }
    }

    // return the source node mapped to a target stop
    DSnap get_source(int target) const { return this->target_mapping[target]; }
};

//*******************************************
// read and write node-mapping
//*******************************************

template <>
struct ReadWrite<NodeMapping>
{
    NodeMapping read(BufferReader& reader)
    {
        auto source_mapping = reader.read<StaticTable<Snap>>();
        auto target_mapping = reader.read<std::vector<DSnap>>();
        return {source_mapping, target_mapping};
    }
    void write(BufferWriter& writer, const NodeMapping& mapping)
    {
        writer.write(mapping.source_mapping);
        writer.write(mapping.target_mapping);
    }
};
