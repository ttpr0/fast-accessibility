#pragma once

#include <unordered_map>
#include <vector>

#include "../../algorithm/connected_components.h"
#include "../base/graph_base.h"
#include "../base_graph.h"
#include "../mod/graph_base_mod.h"
#include "../weights/weighting.h"

std::shared_ptr<GraphBase> remove_unconnected(std::shared_ptr<GraphBase> base)
{
    auto weight = build_equal_weighting(*base);
    auto index = build_base_index(*base);
    Graph graph = build_base_graph(base, weight, index);

    auto comps = connected_components(graph);

    std::unordered_map<int, int> counts;
    for (int i = 0; i < graph.nodeCount(); i++) {
        int comp = comps[i];
        if (counts.contains(comp)) {
            counts[comp] = counts[comp] + 1;
        } else {
            counts[comp] = 1;
        }
    }
    int max_comp = -1;
    int max_count = -1;
    for (auto [comp, count] : counts) {
        if (max_count == -1) {
            max_comp = comp;
            max_count = count;
        } else {
            if (count > max_count) {
                max_comp = comp;
                max_count = count;
            }
        }
    }

    std::vector<int> remove;
    for (int i = 0; i < graph.nodeCount(); i++) {
        int comp = comps[i];
        if (comp != max_comp) {
            remove.push_back(i);
        }
    }

    return _remove_nodes(*base, remove);
}
