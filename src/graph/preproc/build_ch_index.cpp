#include <algorithm>

#include "../explorer/ch_explorer.h"
#include "./build_ch_index.h"

std::shared_ptr<_CHIndex> build_ch_index(GraphBase& base, Weighting& weight, CHData& ch, _IDMapping& id_mapping)
{
    std::vector<Shortcut> fwd_down_edges;
    std::vector<Shortcut> bwd_down_edges;

    CHGraphExplorer explorer = {base, weight, ch, id_mapping};

    for (int i = 0; i < base.nodeCount(); i++) {
        int this_id = i;
        int count = 0;
        explorer.forAdjacentEdges(this_id, Direction::FORWARD, Adjacency::ADJACENT_DOWNWARDS, [&explorer, &fwd_down_edges, &count, &this_id](EdgeRef ref) {
            int other_id = ref.other_id;
            int weight = explorer.getEdgeWeight(ref);
            Shortcut shc = {this_id, other_id, weight};
            shc.payload.set(0, (short)1);
            shc.payload.set(2, false);
            fwd_down_edges.push_back(shc);
            count += 1;
        });
        if (count > 16) {
            for (int j = fwd_down_edges.size() - count; j < fwd_down_edges.size(); j++) {
                auto ch_edge = fwd_down_edges[j];
                ch_edge.payload.set(0, (short)count);
                ch_edge.payload.set(2, true);
                fwd_down_edges[j] = ch_edge;
            }
        }

        count = 0;
        explorer.forAdjacentEdges(this_id, Direction::BACKWARD, Adjacency::ADJACENT_DOWNWARDS, [&explorer, &bwd_down_edges, &count, &this_id](EdgeRef ref) {
            int other_id = ref.other_id;
            int weight = explorer.getEdgeWeight(ref);
            Shortcut shc = {this_id, other_id, weight};
            shc.payload.set(0, (short)1);
            shc.payload.set(2, false);
            bwd_down_edges.push_back(shc);
            count += 1;
        });
        if (count > 16) {
            for (int j = bwd_down_edges.size() - count; j < bwd_down_edges.size(); j++) {
                auto ch_edge = bwd_down_edges[j];
                ch_edge.payload.set(0, (short)count);
                ch_edge.payload.set(2, true);
                bwd_down_edges[j] = ch_edge;
            }
        }
    }

    auto sort_by_level = [&ch](const Shortcut& edge_a, const Shortcut& edge_b) {
        auto level_a = ch.getNodeLevel(edge_a.from);
        auto level_b = ch.getNodeLevel(edge_b.from);
        return level_a > level_b;
    };
    std::sort(fwd_down_edges.begin(), fwd_down_edges.end(), sort_by_level);
    std::sort(bwd_down_edges.begin(), bwd_down_edges.end(), sort_by_level);

    return std::make_shared<_CHIndex>(std::move(fwd_down_edges), std::move(bwd_down_edges));
}

std::shared_ptr<_CHIndex> build_ch_index_2(GraphBase& base, Weighting& weight, CHData& ch, Partition& partition, _IDMapping& id_mapping)
{
    CHGraphExplorer explorer = {base, weight, ch, id_mapping};
    auto get_tile = [&partition, &ch, &id_mapping](int node) {
        int m_node = id_mapping.get_source(node);
        return partition.get_node_tile(m_node);
    };

    // get border nodes
    std::vector<bool> is_border(base.nodeCount());
    for (int i = 0; i < base.nodeCount(); i++) {
        explorer.forAdjacentEdges(i, FORWARD, ADJACENT_ALL, [&](EdgeRef ref) {
            if (get_tile(i) != get_tile(ref.other_id)) {
                is_border[i] = true;
            }
        });
        explorer.forAdjacentEdges(i, BACKWARD, ADJACENT_ALL, [&](EdgeRef ref) {
            if (get_tile(i) != get_tile(ref.other_id)) {
                is_border[i] = true;
            }
        });
    }

    // initialize down edges lists
    std::vector<Shortcut> fwd_down_edges;
    std::vector<Shortcut> bwd_down_edges;
    int border_count = 0;

    // add overlay down-edges
    Shortcut dummy = {0, 0, 0};
    dummy.payload.set(0, (short)0);
    dummy.payload.set(2, true);
    fwd_down_edges.push_back(dummy);
    bwd_down_edges.push_back(dummy);
    std::unordered_map<short, std::vector<Shortcut>> fwd_other_edges;
    std::unordered_map<short, std::vector<Shortcut>> bwd_other_edges;
    for (int i = 0; i < base.nodeCount(); i++) {
        int this_id = i;
        short this_tile = get_tile(this_id);
        if (!is_border[this_id]) {
            border_count = i + 1;
            break;
        }
        explorer.forAdjacentEdges(this_id, Direction::FORWARD, Adjacency::ADJACENT_DOWNWARDS, [&](EdgeRef ref) {
            int other_id = ref.other_id;
            short other_tile = get_tile(other_id);
            Shortcut edge = {this_id, other_id, explorer.getEdgeWeight(ref)};
            edge.payload.set(0, other_tile);
            edge.payload.set(2, false);
            if (!is_border[other_id]) {
                std::vector<Shortcut> edges;
                if (fwd_other_edges.contains(this_tile)) {
                    edges = fwd_other_edges[this_tile];
                }
                edges.push_back(edge);
                fwd_other_edges[this_tile] = edges;
            } else {
                fwd_down_edges.push_back(edge);
            }
        });
        explorer.forAdjacentEdges(this_id, Direction::BACKWARD, Adjacency::ADJACENT_DOWNWARDS, [&](EdgeRef ref) {
            int other_id = ref.other_id;
            short other_tile = get_tile(other_id);
            Shortcut edge = {this_id, other_id, explorer.getEdgeWeight(ref)};
            edge.payload.set(0, other_tile);
            edge.payload.set(2, false);
            if (!is_border[other_id]) {
                std::vector<Shortcut> edges;
                if (bwd_other_edges.contains(this_tile)) {
                    edges = bwd_other_edges[this_tile];
                }
                edges.push_back(edge);
                bwd_other_edges[this_tile] = edges;
            } else {
                bwd_down_edges.push_back(edge);
            }
        });
    }
    // add other down edges
    short curr_tile = -1;
    for (int i = border_count; i < base.nodeCount(); i++) {
        int this_id = i;
        short this_tile = get_tile(this_id);
        if (this_tile != curr_tile) {
            Shortcut dummy = {0, 0, 0};
            dummy.payload.set(0, this_tile);
            dummy.payload.set(2, true);
            fwd_down_edges.push_back(dummy);
            bwd_down_edges.push_back(dummy);
            curr_tile = this_tile;
            if (fwd_other_edges.contains(this_tile)) {
                auto& edges = fwd_other_edges[this_tile];
                for (auto edge : edges) {
                    fwd_down_edges.push_back(edge);
                }
            }
            if (bwd_other_edges.contains(this_tile)) {
                auto& edges = bwd_other_edges[this_tile];
                for (auto edge : edges) {
                    bwd_down_edges.push_back(edge);
                }
            }
        }
        explorer.forAdjacentEdges(this_id, Direction::FORWARD, Adjacency::ADJACENT_DOWNWARDS, [&](EdgeRef ref) {
            int other_id = ref.other_id;
            Shortcut shc = {this_id, other_id, explorer.getEdgeWeight(ref)};
            fwd_down_edges.push_back(shc);
        });
        explorer.forAdjacentEdges(this_id, Direction::BACKWARD, Adjacency::ADJACENT_DOWNWARDS, [&](EdgeRef ref) {
            int other_id = ref.other_id;
            Shortcut shc = {this_id, other_id, explorer.getEdgeWeight(ref)};
            bwd_down_edges.push_back(shc);
        });
    }

    // set count in dummy edges
    int fwd_id = 0;
    int fwd_count = 0;
    for (int i = 0; i < fwd_down_edges.size(); i++) {
        Shortcut edge = fwd_down_edges[i];
        bool is_dummy = edge.payload.get<bool>(2);
        if (is_dummy) {
            // set count in previous dummy
            fwd_down_edges[fwd_id].to = fwd_count;
            // reset count
            fwd_id = i;
            fwd_count = 0;
            continue;
        }
        fwd_count += 1;
    }
    fwd_down_edges[fwd_id].to = fwd_count;
    int bwd_id = 0;
    int bwd_count = 0;
    for (int i = 0; i < bwd_down_edges.size(); i++) {
        Shortcut edge = bwd_down_edges[i];
        bool is_dummy = edge.payload.get<bool>(2);
        if (is_dummy) {
            // set count in previous dummy
            bwd_down_edges[bwd_id].to = bwd_count;
            // reset count
            bwd_id = i;
            bwd_count = 0;
            continue;
        }
        bwd_count += 1;
    }
    bwd_down_edges[bwd_id].to = bwd_count;

    // add to graph
    return std::make_shared<_CHIndex>(std::move(fwd_down_edges), std::move(bwd_down_edges));
}
