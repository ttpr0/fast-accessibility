#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "../util/flags.h"

struct BFSFlag
{
    bool fwd_visited;
    bool bwd_visited;
};

// computes one-to-all distances using forward-dijkstra
void calc_bidirect_bfs(IGraph& g, int start, Flags<BFSFlag>& flags)
{
    std::queue<int> fwd_queue;
    std::queue<int> bwd_queue;
    fwd_queue.push(start);
    bwd_queue.push(start);

    bool fwd_finished = false;
    bool bwd_finished = false;
    while (true) {
        if (fwd_finished && bwd_finished) {
            break;
        }

        if (!fwd_finished) {
            if (fwd_queue.empty()) {
                fwd_finished = true;
            } else {
                auto curr_id = fwd_queue.front();
                fwd_queue.pop();
                auto& curr_flag = flags[curr_id];
                if (!curr_flag.fwd_visited) {
                    curr_flag.fwd_visited = true;
                    g.forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&flags, &g, &fwd_queue, &bwd_finished, &curr_flag](EdgeRef ref) {
                        if (ref.isShortcut()) {
                            return;
                        }
                        int other_id = ref.other_id;
                        if (other_id < 0 || other_id > g.nodeCount()) {
                            printf("test 3");
                        }
                        auto& other_flag = flags[other_id];
                        if (other_flag.fwd_visited) {
                            return;
                        }
                        if (!other_flag.bwd_visited && bwd_finished) {
                            return;
                        }
                        fwd_queue.push(other_id);
                    });
                }
            }
        }

        if (!bwd_finished) {
            if (bwd_queue.empty()) {
                bwd_finished = true;
            } else {
                auto curr_id = bwd_queue.front();
                bwd_queue.pop();
                auto& curr_flag = flags[curr_id];
                if (!curr_flag.bwd_visited) {
                    curr_flag.bwd_visited = true;
                    g.forAdjacentEdges(curr_id, Direction::BACKWARD, Adjacency::ADJACENT_EDGES, [&flags, &g, &bwd_queue, &fwd_finished, &curr_flag](EdgeRef ref) {
                        if (ref.isShortcut()) {
                            return;
                        }
                        int other_id = ref.other_id;
                        if (other_id < 0 || other_id > g.nodeCount()) {
                            printf("test 3");
                        }
                        auto& other_flag = flags[other_id];
                        if (other_flag.bwd_visited) {
                            return;
                        }
                        if (!other_flag.fwd_visited && fwd_finished) {
                            return;
                        }
                        bwd_queue.push(other_id);
                    });
                }
            }
        }
    }
}
