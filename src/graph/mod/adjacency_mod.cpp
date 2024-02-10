#include <memory>

#include "./adjacency_mod.h"

AdjacencyArray _reorder_nodes(const AdjacencyArray& tiled, const std::vector<int>& mapping)
{
    std::vector<_NodeEntry> node_refs(tiled.node_entries.size());
    for (int i = 0; i < tiled.node_entries.size(); i++) {
        int m_id = mapping[i];
        node_refs[m_id] = tiled.node_entries[i];
    }

    std::vector<_EdgeEntry> fwd_edge_refs(tiled.fwd_edge_entries.size());
    std::vector<_EdgeEntry> bwd_edge_refs(tiled.bwd_edge_entries.size());

    int fwd_start = 0;
    int bwd_start = 0;
    for (int i = 0; i < node_refs.size(); i++) {
        auto& node_ref = node_refs[i];
        short fwd_count = 0;
        int start = node_ref.fwd_start;
        for (int j = 0; j < node_ref.fwd_count; j++) {
            auto ref = tiled.fwd_edge_entries[start + j];
            ref.other_id = mapping[ref.other_id];
            fwd_edge_refs[fwd_start + fwd_count] = ref;
            fwd_count += 1;
        }
        node_ref.fwd_start = fwd_start;
        node_ref.fwd_count = fwd_count;
        fwd_start += fwd_count;

        short bwd_count = 0;
        start = node_ref.bwd_start;
        for (int j = 0; j < node_ref.bwd_count; j++) {
            auto ref = tiled.bwd_edge_entries[start + j];
            ref.other_id = mapping[ref.other_id];
            bwd_edge_refs[bwd_start + bwd_count] = ref;
            bwd_count += 1;
        }
        node_ref.bwd_start = bwd_start;
        node_ref.bwd_count = bwd_count;
        bwd_start += bwd_count;
    }

    return {node_refs, fwd_edge_refs, bwd_edge_refs};
}
