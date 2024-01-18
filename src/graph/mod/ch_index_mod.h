#pragma once

#include <memory>
#include <vector>

#include "../speed_ups/ch_index.h"
#include "./adjacency_mod.h"

// reorders nodes,
// mapping: old id -> new id
static std::shared_ptr<_CHIndex> _reorder_nodes(const _CHIndex& index, const std::vector<int>& mapping)
{
    std::vector<Shortcut> new_fwd_shortcuts(index.fwd_down_edges.size());
    for (int i = 0; i < index.fwd_down_edges.size(); i++) {
        auto shc = index.fwd_down_edges[i];
        new_fwd_shortcuts.push_back({
            .from = mapping[shc.from],
            .to = mapping[shc.to],
            .weight = shc.weight,
            .payload = shc.payload,
        });
    }
    std::vector<Shortcut> new_bwd_shortcuts(index.bwd_down_edges.size());
    for (int i = 0; i < index.bwd_down_edges.size(); i++) {
        auto shc = index.bwd_down_edges[i];
        new_bwd_shortcuts.push_back({
            .from = mapping[shc.from],
            .to = mapping[shc.to],
            .weight = shc.weight,
            .payload = shc.payload,
        });
    }

    return std::make_shared<_CHIndex>(std::move(new_fwd_shortcuts), std::move(new_bwd_shortcuts));
}
