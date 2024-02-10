#include <memory>

#include "./adjacency_mod.h"
#include "./overlay_mod.h"

std::shared_ptr<TiledData> _reorder_nodes(const TiledData& tiled, const std::vector<int>& mapping)
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

std::shared_ptr<_CellIndex> _reorder_nodes(const _CellIndex& index, const std::vector<int>& mapping)
{
    std::unordered_map<short, std::vector<Shortcut>> new_fwd_index;
    for (auto [tile, shortcuts] : index.fwd_index_edges) {
        std::vector<Shortcut> new_shortcuts(shortcuts.size());
        for (int i = 0; i < shortcuts.size(); i++) {
            auto& shc = shortcuts[i];
            new_shortcuts[i] = {
                .from = mapping[shc.from],
                .to = mapping[shc.to],
                .weight = shc.weight,
                .payload = shc.payload,
            };
        }
        new_fwd_index[tile] = new_shortcuts;
    }
    std::unordered_map<short, std::vector<Shortcut>> new_bwd_index;
    for (auto [tile, shortcuts] : index.bwd_index_edges) {
        std::vector<Shortcut> new_shortcuts(shortcuts.size());
        for (int i = 0; i < shortcuts.size(); i++) {
            auto& shc = shortcuts[i];
            new_shortcuts[i] = {
                .from = mapping[shc.from],
                .to = mapping[shc.to],
                .weight = shc.weight,
                .payload = shc.payload,
            };
        }
        new_bwd_index[tile] = new_shortcuts;
    }

    return std::make_shared<_CellIndex>(std::move(new_fwd_index), std::move(new_bwd_index));
}
