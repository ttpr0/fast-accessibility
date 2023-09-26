#pragma once

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../graph/graph.h"
#include "./pq_item.h"

// simple dijkstra with range and target count restriction
void calcRestrictedRangeDijkstra(IGraph* g, int start, std::vector<int>& dist, std::vector<bool>& visited, int max_range, std::vector<bool>& targets, int target_count)
{
    dist[start] = 0;

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
    int found_count = 0;
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        if (visited[curr_id]) {
            continue;
        }
        visited[curr_id] = true;
        if (targets[curr_id]) {
            found_count += 1;
            if (found_count == target_count) {
                break;
            }
        }
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, [&dist, &visited, &explorer, &heap, &curr_id, &max_range](EdgeRef ref) {
            if (ref.isShortcut()) {
                return;
            }
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        });
    }
}

// simple PHAST
void calcPHAST(ICHGraph* g, int start, std::vector<int>& dist, std::vector<bool>& visited)
{
    dist[start] = 0;

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        if (visited[curr_id]) {
            continue;
        }
        visited[curr_id] = true;
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &curr_id](EdgeRef ref) {
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        });
    }

    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    int length = down_edges.size();
    for (int i = 0; i < length; i++) {
        auto edge = down_edges[i];
        int curr_len = dist[edge.from];
        int new_len = curr_len + edge.weight;
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }
}

// simple RPHAST
void calcRPHAST(ICHGraph* g, int start, std::vector<int>& dist, std::vector<bool>& visited, std::vector<CHEdge>& down_edges_subset)
{
    dist[start] = 0;

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        if (visited[curr_id]) {
            continue;
        }
        visited[curr_id] = true;
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &curr_id](EdgeRef ref) {
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        });
    }

    int length = down_edges_subset.size();
    for (int i = 0; i < length; i++) {
        auto edge = down_edges_subset[i];
        int curr_len = dist[edge.from];
        int new_len = curr_len + edge.weight;
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }
}

// RPHAST subset selection
std::vector<CHEdge> preprocessRPHAST(ICHGraph* g, std::vector<bool>& targets)
{
    std::queue<int> node_queue;
    for (int i = 0; i < g->nodeCount(); i++) {
        if (targets[i]) {
            node_queue.push(i);
        }
    }

    auto explorer = g->getGraphExplorer();
    std::vector<bool> graph_subset(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        graph_subset[i] = false;
    }
    while (true) {
        if (node_queue.size() == 0) {
            break;
        }
        auto node = node_queue.front();
        node_queue.pop();
        if (graph_subset[node]) {
            continue;
        }
        graph_subset[node] = true;
        short node_level = g->getNodeLevel(node);
        explorer->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            node_queue.push(ref.other_id);
        });
    }
    // selecting subset of downward edges for linear sweep
    std::vector<CHEdge> down_edges_subset;
    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    for (int i = 0; i < down_edges.size(); i++) {
        auto edge = down_edges[i];
        if (!graph_subset[edge.from]) {
            continue;
        }
        down_edges_subset.push_back(edge);
    }

    return std::move(down_edges_subset);
}

// PHAST with simple range restriction
void calcRangePHAST(ICHGraph* g, int start, std::vector<int>& dist, std::vector<bool>& visited, int max_range)
{
    dist[start] = 0;

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        if (visited[curr_id]) {
            continue;
        }
        visited[curr_id] = true;
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &curr_id, &max_range](EdgeRef ref) {
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        });
    }

    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    int length = down_edges.size();
    for (int i = 0; i < length; i++) {
        auto edge = down_edges[i];
        int curr_len = dist[edge.from];
        int new_len = curr_len + edge.weight;
        if (new_len > max_range) {
            continue;
        }
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }
}

// RPHAST with simple range restriction
void calcRangeRPHAST(ICHGraph* g, int start, std::vector<int>& dist, std::vector<bool>& visited, int max_range, std::vector<CHEdge>& down_edges_subset)
{
    dist[start] = 0;

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        if (visited[curr_id]) {
            continue;
        }
        visited[curr_id] = true;
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &curr_id, &max_range](EdgeRef ref) {
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        });
    }

    int length = down_edges_subset.size();
    for (int i = 0; i < length; i++) {
        auto edge = down_edges_subset[i];
        int curr_len = dist[edge.from];
        int new_len = curr_len + edge.weight;
        if (new_len > max_range) {
            continue;
        }
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
        }
    }
}

// RPHAST subset selection using priority queue
std::vector<CHEdge> preprocessRangeRPHAST(ICHGraph* g, int max_range, std::vector<bool>& targets)
{
    std::priority_queue<pq_item> node_queue;
    std::vector<int> lengths(g->nodeCount());
    std::vector<bool> graph_subset(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        graph_subset[i] = false;
        lengths[i] = 100000000;
        if (targets[i]) {
            lengths[i] = 0;
            node_queue.push({i, 0});
        }
    }

    // select graph subset by marking visited nodes
    auto explorer = g->getGraphExplorer();
    while (true) {
        if (node_queue.empty()) {
            break;
        }
        auto item = node_queue.top();
        int node = item.node;
        node_queue.pop();
        if (graph_subset[node]) {
            continue;
        }
        graph_subset[node] = true;
        int node_len = lengths[node];
        explorer->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &explorer, node_len, &lengths, max_range, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            int new_len = node_len + explorer->getEdgeWeight(ref);
            if (new_len > max_range) {
                return;
            }
            if (new_len < lengths[ref.other_id]) {
                lengths[ref.other_id] = new_len;
                node_queue.push({ref.other_id, new_len});
            }
        });
    }
    // selecting subset of downward edges for linear sweep
    std::vector<CHEdge> down_edges_subset;
    const std::vector<CHEdge>& down_edges = g->getDownEdges(Direction::FORWARD);
    for (int i = 0; i < down_edges.size(); i++) {
        auto edge = down_edges[i];
        if (!graph_subset[edge.from]) {
            continue;
        }
        down_edges_subset.push_back(edge);
    }

    return std::move(down_edges_subset);
}

// PHAST with graph-partitioning
void calcPHAST2(CHGraph2* g, int start, std::vector<int>& dist, std::vector<bool>& visited, int max_range)
{
    dist[start] = 0;

    int tile_count = g->tileCount();
    std::vector<bool> active_tiles(tile_count);
    for (int i = 0; i < tile_count; i++) {
        active_tiles[i] = false;
    }

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        if (visited[curr_id]) {
            continue;
        }
        visited[curr_id] = true;
        short curr_tile = g->getNodeTile(curr_id);
        active_tiles[curr_tile] = true;
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &curr_id, &max_range](EdgeRef ref) {
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        });
    }
    // iterative down sweep
    const std::vector<CHEdge4>& down_edges = g->getDownEdges4(Direction::FORWARD);
    CHEdge4 overlay_dummy = down_edges[0];
    int overlay_start = 1;
    int overlay_end = 1 + overlay_dummy.to;
    for (int i = overlay_start; i < overlay_end; i++) {
        CHEdge4 edge = down_edges[i];
        int curr_len = dist[edge.from];
        int new_len = curr_len + edge.weight;
        if (new_len > max_range) {
            continue;
        }
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
            active_tiles[edge.to_tile] = true;
        }
    }
    for (int i = overlay_end; i < down_edges.size(); i++) {
        CHEdge4 curr_dummy = down_edges[i];
        int curr_tile = curr_dummy.to_tile;
        int curr_count = curr_dummy.to;
        if (active_tiles[curr_tile]) {
            int tile_start = i + 1;
            int tile_end = i + 1 + curr_count;
            for (int j = tile_start; j < tile_end; j++) {
                CHEdge4 edge = down_edges[j];
                int curr_len = dist[edge.from];
                int new_len = curr_len + edge.weight;
                if (new_len > max_range) {
                    continue;
                }
                if (dist[edge.to] > new_len) {
                    dist[edge.to] = new_len;
                }
            }
        }
        i += curr_count;
    }
}

// RPHAST with graph-partitioning
void calcRPHAST2(CHGraph2* g, int start, std::vector<int>& dist, std::vector<bool>& visited, int max_range, std::vector<CHEdge4>& down_edges_subset)
{
    dist[start] = 0;

    int tile_count = g->tileCount();
    std::vector<bool> active_tiles(tile_count);
    for (int i = 0; i < tile_count; i++) {
        active_tiles[i] = false;
    }

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        if (visited[curr_id]) {
            continue;
        }
        visited[curr_id] = true;
        short curr_tile = g->getNodeTile(curr_id);
        active_tiles[curr_tile] = true;
        explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_UPWARDS, [&dist, &visited, &explorer, &heap, &curr_id, &max_range](EdgeRef ref) {
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        });
    }
    // iterative down sweep
    CHEdge4 overlay_dummy = down_edges_subset[0];
    int overlay_start = 1;
    int overlay_end = 1 + overlay_dummy.to;
    for (int i = overlay_start; i < overlay_end; i++) {
        CHEdge4 edge = down_edges_subset[i];
        int curr_len = dist[edge.from];
        int new_len = curr_len + edge.weight;
        if (new_len > max_range) {
            continue;
        }
        if (dist[edge.to] > new_len) {
            dist[edge.to] = new_len;
            active_tiles[edge.to_tile] = true;
        }
    }
    // iterative down sweep
    for (int i = overlay_end; i < down_edges_subset.size(); i++) {
        CHEdge4 curr_dummy = down_edges_subset[i];
        int curr_tile = curr_dummy.to_tile;
        int curr_count = curr_dummy.to;
        if (active_tiles[curr_tile]) {
            int tile_start = i + 1;
            int tile_end = i + 1 + curr_count;
            for (int j = tile_start; j < tile_end; j++) {
                CHEdge4 edge = down_edges_subset[j];
                int curr_len = dist[edge.from];
                int new_len = curr_len + edge.weight;
                if (new_len > max_range) {
                    continue;
                }
                if (dist[edge.to] > new_len) {
                    dist[edge.to] = new_len;
                }
            }
        }
        i += curr_count;
    }
}

// RPHAST preprocessing (including updates on dummy edges)
std::vector<CHEdge4> preprocessRPHAST2(CHGraph2* g, std::vector<bool>& targets)
{
    std::queue<int> node_queue;
    for (int i = 0; i < g->nodeCount(); i++) {
        if (targets[i]) {
            node_queue.push(i);
        }
    }

    auto explorer = g->getGraphExplorer();
    std::vector<bool> graph_subset(g->nodeCount());
    for (int i = 0; i < g->nodeCount(); i++) {
        graph_subset[i] = false;
    }
    while (true) {
        if (node_queue.size() == 0) {
            break;
        }
        auto node = node_queue.front();
        node_queue.pop();
        if (graph_subset[node]) {
            continue;
        }
        graph_subset[node] = true;
        short node_level = g->getNodeLevel(node);
        explorer->forAdjacentEdges(node, Direction::BACKWARD, Adjacency::ADJACENT_UPWARDS, [&graph_subset, &node_queue](EdgeRef ref) {
            if (graph_subset[ref.other_id]) {
                return;
            }
            node_queue.push(ref.other_id);
        });
    }
    // selecting subset of downward edges for linear sweep
    std::vector<CHEdge4> down_edges_subset;
    const std::vector<CHEdge4>& down_edges = g->getDownEdges4(Direction::FORWARD);
    int curr_id = 0;
    int curr_count = 0;
    down_edges_subset.push_back(down_edges[0]);
    for (int i = 0; i < down_edges.size(); i++) {
        auto edge = down_edges[i];
        if (edge.is_dummy) {
            if (curr_count == 0) {
                down_edges_subset[curr_id] = edge;
                curr_count = 0;
                continue;
            }
            down_edges_subset[curr_id].to = curr_count;
            curr_id = down_edges_subset.size();
            curr_count = 0;
            down_edges_subset.push_back(edge);
            continue;
        }
        if (!graph_subset[edge.from]) {
            continue;
        }
        down_edges_subset.push_back(edge);
        curr_count += 1;
    }
    down_edges_subset[curr_id].to = curr_count;

    return std::move(down_edges_subset);
}

void calcGRASP(ITiledGraph* g, int start, std::vector<int>& dist, std::vector<bool>& visited, int max_range)
{
    dist[start] = 0;

    short start_tile = g->getNodeTile(start);

    int tile_count = g->tileCount();
    std::vector<bool> active_tiles(tile_count);
    std::vector<bool> found_tiles(tile_count);
    for (int i = 0; i < tile_count; i++) {
        active_tiles[i] = false;
        found_tiles[i] = false;
    }

    auto explorer = g->getGraphExplorer();

    std::priority_queue<pq_item> heap;
    heap.push({start, 0});

    // routing loop
    while (true) {
        if (heap.empty()) {
            break;
        }
        auto item = heap.top();
        int curr_id = item.node;
        heap.pop();
        if (visited[curr_id]) {
            continue;
        }
        visited[curr_id] = true;
        short curr_tile = g->getNodeTile(curr_id);
        auto handler = [&dist, &visited, &explorer, &heap, &max_range, &curr_id](EdgeRef ref) {
            int other_id = ref.other_id;
            if (visited[other_id]) {
                return;
            }
            int new_length = dist[curr_id] + explorer->getEdgeWeight(ref);
            if (new_length > max_range) {
                return;
            }
            if (dist[other_id] > new_length) {
                dist[other_id] = new_length;
                heap.push({other_id, new_length});
            }
        };
        if (curr_tile == start_tile) {
            explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_EDGES, handler);
        } else {
            found_tiles[curr_tile] = true;
            explorer->forAdjacentEdges(curr_id, Direction::FORWARD, Adjacency::ADJACENT_SKIP, handler);
        }
    }
    for (int i = 0; i < tile_count; i++) {
        if (!active_tiles[i] || !found_tiles[i]) {
            continue;
        }
        auto& down_edges = g->getIndexEdges(i, Direction::FORWARD);
        for (int j = 0; j < down_edges.size(); j++) {
            TiledSHEdge edge = down_edges[j];
            int curr_len = dist[edge.from];
            int new_len = curr_len + edge.weight;
            if (dist[edge.to] > new_len) {
                dist[edge.to] = new_len;
            }
        }
    }
}