#pragma once

#include <cstdio>
#include <vector>

#include "./bidirect_bfs.h"
#include "../util/flags.h"

// Group closely connected components.
std::vector<int> connected_components(IGraph& graph)
{
    // compute closely connected components
    std::vector<int> groups(graph.nodeCount());
    for (int i = 0; i < graph.nodeCount(); i++) {
        groups[i] = 0;
    }
    int group = 1;
    Flags<BFSFlag> flags(graph.nodeCount(), {false, false});
    for (int i = 0; i < graph.nodeCount(); i++) {
        if (groups[i] != 0) {
            continue;
        }
        printf("iteration: %i \n", group);
        int start = i;
        flags.soft_reset();
        calc_bidirect_bfs(graph, start, flags);
        for (int j = 0; j < graph.nodeCount(); j++) {
            auto& flag = flags[j];
            if (flag.fwd_visited && flag.bwd_visited) {
                if (groups[j] != 0) {
                    printf("failure 1 \n");
                }
                groups[j] = group;
            }
        }
        group += 1;
    }
    return groups;
}
