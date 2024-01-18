#pragma once

#include <memory>
#include <vector>

#include "../speed_ups/tiled_data.h"
#include "./adjacency_mod.h"

// reorders nodes,
// mapping: old id -> new id
static std::shared_ptr<TiledData> _reorder_nodes(const TiledData& tiled, const std::vector<int>& mapping)
{
    std::vector<Shortcut> new_shortcuts(tiled.shortcutCount());
    for (int i = 0; i < tiled.shortcutCount(); i++) {
        auto shc = tiled.getShortcut(i);
        new_shortcuts[i] = {
            .from = mapping[shc.from],
            .to = mapping[shc.to],
            .weight = shc.weight,
            .payload = shc.payload,
        };
    }

    auto skip_topology = _reorder_nodes(tiled.topology, mapping);

    return std::make_shared<TiledData>(std::move(new_shortcuts), std::move(skip_topology), tiled.edge_types);
}
