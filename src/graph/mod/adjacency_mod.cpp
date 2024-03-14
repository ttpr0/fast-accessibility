#include <memory>

#include "./adjacency_mod.h"

AdjacencyArray _reorder_nodes(const AdjacencyArray& tiled, const std::vector<int>& mapping)
{
    auto new_fwd_entries = tiled.fwd_entries.reorderEntries(mapping, [&mapping](_EdgeEntry entry) {
        entry.other_id = mapping[entry.other_id];
        return entry;
    });
    auto new_bwd_entries = tiled.fwd_entries.reorderEntries(mapping, [&mapping](_EdgeEntry entry) {
        entry.other_id = mapping[entry.other_id];
        return entry;
    });

    return {new_fwd_entries, new_bwd_entries};
}
