#pragma once

#include <memory>
#include <vector>

#include "../speed_ups/cell_index.h"
#include "./adjacency_mod.h"

// reorders nodes,
// mapping: old id -> new id
static std::shared_ptr<_CellIndex> _reorder_nodes(const _CellIndex& index, const std::vector<int>& mapping)
{
    std::unordered_map<short, std::vector<Shortcut>> new_fwd_index;
    for (auto [tile, shortcuts] : index.fwd_index_edges) {
        std::vector<Shortcut> new_shortcuts(shortcuts.size());
        for (auto shc : shortcuts) {
            new_shortcuts.push_back({
                .from = mapping[shc.from],
                .to = mapping[shc.to],
                .weight = shc.weight,
                .payload = shc.payload,
            });
        }
        new_fwd_index[tile] = new_shortcuts;
    }
    std::unordered_map<short, std::vector<Shortcut>> new_bwd_index;
    for (auto [tile, shortcuts] : index.bwd_index_edges) {
        std::vector<Shortcut> new_shortcuts(shortcuts.size());
        for (auto shc : shortcuts) {
            new_shortcuts.push_back({
                .from = mapping[shc.from],
                .to = mapping[shc.to],
                .weight = shc.weight,
                .payload = shc.payload,
            });
        }
        new_bwd_index[tile] = new_shortcuts;
    }

    return std::make_shared<_CellIndex>(std::move(new_fwd_index), std::move(new_bwd_index));
}
