#pragma once

#include <algorithm>
#include <array>
#include <queue>
#include <tuple>
#include <vector>

#include "../../../algorithm/util.h"
#include "../../comps/partition.h"
#include "../../graph.h"
#include "../../structs/enums.h"
#include "../../structs/structs.h"

//*******************************************
// preprocessing utility methods
//*******************************************

struct _Flag
{
    int pathlength;
    int prevEdge;
    bool visited;
};

// computes shortest paths from start restricted to tile
void _calc_shortest_paths(IGraph& graph, int start, Flags<_Flag> flags, Partition& partition)
{
    auto& explorer = graph.getGraphExplorer();
    std::priority_queue<pq_item> heap;

    flags[start] = {.pathlength = 0, .prevEdge = -1, .visited = false};
    heap.push({start, 0});

    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        auto& curr_flag = flags[curr_id];
        if (curr_flag.visited) {
            continue;
        }
        curr_flag.visited = true;
        short curr_tile = partition.get_node_tile(curr_id);
        explorer.forAdjacentEdges(curr_id, FORWARD, ADJACENT_ALL, [&heap, &explorer, &flags, &curr_tile, &partition, &curr_flag](EdgeRef ref) {
            if (!ref.isEdge()) {
                return;
            }
            int other_id = ref.other_id;
            short other_tile = partition.get_node_tile(other_id);
            if (other_tile != curr_tile) {
                return;
            }
            auto& other_flag = flags[other_id];
            if (other_flag.visited) {
                return;
            }
            int weight = explorer.getEdgeWeight(ref);
            int newlength = curr_flag.pathlength + weight;
            if (newlength < other_flag.pathlength) {
                other_flag.pathlength = newlength;
                other_flag.prevEdge = ref.edge_id;
                heap.push({other_id, newlength});
            }
        });
    }
}

// creates topology with cross-border edges (type 10), skip edges (type 20) and shortcuts (type 100)
AdjacencyArray _create_skip_topology(IGraph& graph, std::vector<Shortcut>& shortcuts, std::vector<char>& edge_types)
{
    AdjacencyList dyn_top(graph.nodeCount());

    for (int i = 0; i < graph.edgeCount(); i++) {
        int edge_id = i;
        char edge_typ = edge_types[edge_id];
        if (edge_typ != 10 && edge_typ != 20) {
            continue;
        }
        auto edge = graph.getEdge(edge_id);
        dyn_top.addEdgeEntries(edge.nodeA, edge.nodeB, edge_id, edge_typ);
    }

    for (int i = 0; i < shortcuts.size(); i++) {
        int shc_id = i;
        auto shc = shortcuts[shc_id];
        dyn_top.addEdgeEntries(shc.from, shc.to, shc_id, 100);
    }

    return build_adjacency_array(dyn_top);
}

//*******************************************
// preprocessing utility methods
//*******************************************

// return list of nodes that have at least one cross-border edge
//
// returns in_nodes, out_nodes
std::tuple<std::vector<int>, std::vector<int>> _get_inout_nodes(IGraph& graph, Partition& partition, short tile_id)
{
    std::vector<int> in_list;
    std::vector<int> out_list;

    auto& explorer = graph.getGraphExplorer();
    for (int i = 0; i < graph.nodeCount(); i++) {
        int id = i;
        short tile = partition.get_node_tile(id);
        if (tile != tile_id) {
            continue;
        }
        bool is_added = false;
        explorer.forAdjacentEdges(id, Direction::BACKWARD, Adjacency::ADJACENT_ALL, [id, &in_list, &is_added, &partition, tile](EdgeRef ref) {
            if (is_added) {
                return;
            }
            short other_tile = partition.get_node_tile(ref.other_id);
            if (tile != other_tile) {
                in_list.push_back(id);
                is_added = true;
            }
        });

        is_added = false;
        explorer.forAdjacentEdges(id, Direction::FORWARD, Adjacency::ADJACENT_ALL, [id, &out_list, &is_added, &partition, tile](EdgeRef ref) {
            if (is_added) {
                return;
            }
            short other_tile = partition.get_node_tile(ref.other_id);
            if (tile != other_tile) {
                out_list.push_back(id);
                is_added = true;
            }
        });
    }
    return std::make_tuple(in_list, out_list);
}

// Computes border and interior nodes of graph tile.
//
// If tile doesn't exist arrays will be empty.
std::tuple<std::vector<int>, std::vector<int>> _get_border_interior_nodes(IGraph& graph, Partition& partition, short tile_id)
{
    std::vector<int> border_list;
    std::vector<int> interior_list;

    auto& explorer = graph.getGraphExplorer();
    for (int i = 0; i < graph.nodeCount(); i++) {
        int id = i;
        short tile = partition.get_node_tile(id);
        if (tile != tile_id) {
            continue;
        }
        bool is_border = false;
        explorer.forAdjacentEdges(id, Direction::BACKWARD, Adjacency::ADJACENT_ALL, [id, &is_border, &partition, tile](EdgeRef ref) {
            if (is_border) {
                return;
            }
            short other_tile = partition.get_node_tile(ref.other_id);
            if (tile != other_tile) {
                is_border = true;
            }
        });
        explorer.forAdjacentEdges(id, Direction::FORWARD, Adjacency::ADJACENT_ALL, [id, &is_border, &partition, tile](EdgeRef ref) {
            if (is_border) {
                return;
            }
            short other_tile = partition.get_node_tile(ref.other_id);
            if (tile != other_tile) {
                is_border = true;
            }
        });

        if (is_border) {
            border_list.push_back(id);
        } else {
            interior_list.push_back(id);
        }
    }
    return std::make_tuple(border_list, interior_list);
}

// Computes border nodes of graph.
std::vector<bool> _get_is_border(IGraph& graph, Partition& partition)
{
    std::vector<bool> is_border_list(graph.nodeCount());

    auto& explorer = graph.getGraphExplorer();
    for (int i = 0; i < graph.nodeCount(); i++) {
        int id = i;
        short tile = partition.get_node_tile(id);
        bool is_border = false;
        explorer.forAdjacentEdges(id, Direction::BACKWARD, Adjacency::ADJACENT_ALL, [id, &is_border, &partition, tile](EdgeRef ref) {
            if (is_border) {
                return;
            }
            short other_tile = partition.get_node_tile(ref.other_id);
            if (tile != other_tile) {
                is_border = true;
            }
        });
        explorer.forAdjacentEdges(id, Direction::FORWARD, Adjacency::ADJACENT_ALL, [id, &is_border, &partition, tile](EdgeRef ref) {
            if (is_border) {
                return;
            }
            short other_tile = partition.get_node_tile(ref.other_id);
            if (tile != other_tile) {
                is_border = true;
            }
        });

        if (is_border) {
            is_border_list[i] = true;
        } else {
            is_border_list[i] = false;
        }
    }
    return is_border_list;
}
