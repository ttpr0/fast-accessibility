#pragma once

#include <queue>
#include <span>
#include <vector>

#include "../../graph.h"

struct _Flag_EK
{
    int prev_node;
    int prev_edge;
    bool is_reverse;
    bool is_virtual;
};

class EdmondsKarp
{
public:
    IGraph* g;
    std::vector<short> node_tiles;

    // edge flows stored as 0 or 1;
    //
    // To solve issue with reverse oneway edges, each directed edge of the original graph is viewed as two edges with two flows:
    // first is for the actual edge,
    // second for a "virtual" edge used during BFS reversed to the orignal edge;
    std::vector<std::array<char, 2>> edge_flow;
    short base_tile;
    short source_tile;
    short sink_tile;
    int max_flow;

    std::queue<int> source_queue;
    std::vector<_Flag_EK> bfs_flags;
    std::vector<bool> visited;

    EdmondsKarp() {}

    EdmondsKarp(IGraph* g, std::span<int> sources, short source_tile, std::span<int> sinks, short sink_tile, std::span<int> base_nodes, short base_tile)
        : g(g), base_tile(base_tile), source_tile(source_tile), sink_tile(sink_tile), max_flow(0)
    {
        this->source_queue = std::queue<int>();
        this->node_tiles = std::vector<short>(g->nodeCount(), 0);
        this->bfs_flags = std::vector<_Flag_EK>(g->nodeCount(), {0});
        this->visited = std::vector<bool>(g->nodeCount(), false);
        this->edge_flow = std::vector<std::array<char, 2>>(g->edgeCount(), {0, 0});

        for (auto node : base_nodes) {
            node_tiles[node] = base_tile;
        }
        for (auto source : sources) {
            source_queue.push(source);
            node_tiles[source] = source_tile;
        }
        for (auto sink : sinks) {
            node_tiles[sink] = sink_tile;
        }
    }

    int ComputeMaxFlow()
    {
        while (true) {
            int flow = this->BFS();
            if (flow == 0) {
                break;
            }
            this->max_flow += flow;
        }
        return this->max_flow;
    }

    void ComputeMinCut()
    {
        auto queue = this->source_queue;
        std::vector<bool> visited(this->g->nodeCount());

        // clear visited
        for (int i = 0; i < this->g->nodeCount(); i++) {
            visited[i] = false;
            if (this->node_tiles[i] == this->source_tile) {
                visited[i] = true;
            }
        }

        while (true) {
            if (queue.size() <= 0) {
                break;
            }
            int curr = queue.front();
            queue.pop();
            if (this->node_tiles[curr] == this->sink_tile) {
                throw "this should not happen";
            }
            if (this->node_tiles[curr] == this->base_tile) {
                this->node_tiles[curr] = this->source_tile;
            }

            this->g->forAdjacentEdges(curr, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&visited, this, &queue](EdgeRef ref) {
                if (visited[ref.other_id] || this->node_tiles[ref.other_id] != this->base_tile) {
                    return;
                }
                auto flow = this->edge_flow[ref.edge_id];
                if (flow[0] == 0 || flow[1] == 1) {
                    queue.push(ref.other_id);
                    visited[ref.other_id] = true;
                }
            });
            this->g->forAdjacentEdges(curr, Direction::BACKWARD, Adjacency::ADJACENT_EDGES, [&visited, this, &queue](EdgeRef ref) {
                if (visited[ref.other_id] || this->node_tiles[ref.other_id] != this->base_tile) {
                    return;
                }
                auto flow = this->edge_flow[ref.edge_id];
                if (flow[0] == 1 || flow[1] == 0) {
                    queue.push(ref.other_id);
                    visited[ref.other_id] = true;
                }
            });
        }

        for (int i = 0; i < this->g->nodeCount(); i++) {
            if (this->node_tiles[i] == this->base_tile) {
                this->node_tiles[i] = this->sink_tile;
            }
        }
    }

    std::vector<short>& GetNodeTiles() { return this->node_tiles; }

    // computed bfs on residual graph and returns new flow
    int BFS()
    {
        auto& flags = this->bfs_flags;
        auto& visited = this->visited;
        auto queue = this->source_queue;

        // clear visited
        for (int i = 0; i < visited.size(); i++) {
            if (this->node_tiles[i] == this->source_tile) {
                visited[i] = true;
            } else {
                visited[i] = false;
            }
        }

        int end = -1;
        while (true) {
            if (queue.size() <= 0) {
                break;
            }
            int curr = queue.front();
            queue.pop();
            if (this->node_tiles[curr] == this->sink_tile) {
                end = curr;
                break;
            }

            this->g->forAdjacentEdges(curr, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [curr, &visited, this, &flags, &queue](EdgeRef ref) {
                // check if edge should stil be traversed
                if (visited[ref.other_id]) {
                    return;
                }
                // check if node is part of subgraph
                short tile = this->node_tiles[ref.other_id];
                if (tile != this->base_tile && tile != this->source_tile && tile != this->sink_tile) {
                    return;
                }

                auto flow = this->edge_flow[ref.edge_id];
                if (flow[0] == 0) {
                    auto& other_flag = flags[ref.other_id];
                    other_flag.is_reverse = false;
                    other_flag.is_virtual = false;
                    other_flag.prev_edge = ref.edge_id;
                    other_flag.prev_node = curr;
                    flags[ref.other_id] = other_flag;
                    queue.push(ref.other_id);
                    visited[ref.other_id] = true;
                } else if (flow[1] == 1) {
                    auto& other_flag = flags[ref.other_id];
                    other_flag.is_reverse = true;
                    other_flag.is_virtual = true;
                    other_flag.prev_edge = ref.edge_id;
                    other_flag.prev_node = curr;
                    flags[ref.other_id] = other_flag;
                    queue.push(ref.other_id);
                    visited[ref.other_id] = true;
                }
            });
            this->g->forAdjacentEdges(curr, Direction::BACKWARD, Adjacency::ADJACENT_EDGES, [curr, &visited, this, &flags, &queue](EdgeRef ref) {
                // check if edge should stil be traversed
                if (visited[ref.other_id]) {
                    return;
                }
                // check if node is part of subgraph
                short tile = this->node_tiles[ref.other_id];
                if (tile != this->base_tile && tile != this->source_tile && tile != this->sink_tile) {
                    return;
                }

                auto& flow = this->edge_flow[ref.edge_id];
                if (flow[0] == 1) {
                    auto& other_flag = flags[ref.other_id];
                    other_flag.is_reverse = true;
                    other_flag.is_virtual = false;
                    other_flag.prev_edge = ref.edge_id;
                    other_flag.prev_node = curr;
                    flags[ref.other_id] = other_flag;
                    queue.push(ref.other_id);
                    visited[ref.other_id] = true;
                } else if (flow[1] == 0) {
                    auto& other_flag = flags[ref.other_id];
                    other_flag.is_reverse = false;
                    other_flag.is_virtual = true;
                    other_flag.prev_edge = ref.edge_id;
                    other_flag.prev_node = curr;
                    flags[ref.other_id] = other_flag;
                    queue.push(ref.other_id);
                    visited[ref.other_id] = true;
                }
            });
        }

        if (end == -1) {
            return 0;
        }
        while (true) {
            if (this->node_tiles[end] == this->source_tile) {
                break;
            }
            auto curr_flag = flags[end];
            if (curr_flag.is_reverse) {
                auto& flow = this->edge_flow[curr_flag.prev_edge];
                if (curr_flag.is_virtual) {
                    flow[1] = 0;
                } else {
                    flow[0] = 0;
                }
                this->edge_flow[curr_flag.prev_edge] = flow;
            } else {
                auto& flow = this->edge_flow[curr_flag.prev_edge];
                if (curr_flag.is_virtual) {
                    flow[1] = 1;
                } else {
                    flow[0] = 1;
                }
                this->edge_flow[curr_flag.prev_edge] = flow;
            }
            end = curr_flag.prev_node;
        }
        return 1;
    }
};
