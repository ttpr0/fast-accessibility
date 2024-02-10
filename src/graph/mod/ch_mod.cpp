#include <memory>

#include "./adjacency_mod.h"
#include "./ch_mod.h"

std::shared_ptr<CHData> _reorder_nodes(const CHData& ch, const std::vector<int>& mapping)
{
    std::vector<short> new_levels(ch.node_levels.size());
    for (int i = 0; i < ch.node_levels.size(); i++) {
        int m_id = mapping[i];
        new_levels[m_id] = ch.node_levels[i];
    }

    std::vector<Shortcut> new_shortcuts(ch.shortcutCount());
    for (int i = 0; i < ch.shortcutCount(); i++) {
        auto shc = ch.getShortcut(i);
        new_shortcuts[i] = {
            .from = mapping[shc.from],
            .to = mapping[shc.to],
            .weight = shc.weight,
            .payload = shc.payload,
        };
    }

    auto new_topology = _reorder_nodes(ch.topology, mapping);

    return std::make_shared<CHData>(std::move(new_shortcuts), std::move(new_topology), new_levels);
}

std::shared_ptr<_CHIndex> _reorder_nodes(const _CHIndex& index, const std::vector<int>& mapping)
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
