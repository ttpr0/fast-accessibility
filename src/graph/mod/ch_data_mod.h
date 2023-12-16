#pragma once

#include <memory>
#include <vector>

#include "../speed_ups/ch_data.h"
#include "./adjacency_mod.h"

// reorders nodes,
// mapping: old id -> new id
static std::shared_ptr<CHData> _reorder_nodes(const CHData& ch, const std::vector<int>& mapping)
{
    std::vector<short> new_levels(ch.node_levels.size());
    for (int i = 0; i < ch.node_levels.size(); i++) {
        int m_id = mapping[i];
        new_levels[m_id] = ch.node_levels[i];
    }

    std::vector<Shortcut> new_shortcuts(ch.shortcutCount());
    for (int i = 0; i < ch.shortcutCount(); i++) {
        auto shc = ch.getShortcut(i);
        new_shortcuts.push_back({
            .from = mapping[shc.from],
            .to = mapping[shc.to],
            .weight = shc.weight,
            .payload = shc.payload,
        });
    }

    auto new_topology = _reorder_nodes(ch.topology, mapping);

    return std::make_shared<CHData>(std::move(new_shortcuts), std::move(new_topology), new_levels);
}
