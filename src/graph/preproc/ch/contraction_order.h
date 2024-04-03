#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>

#include "../../../util/pq_item.h"
#include "./local_search.h"
#include "./preproc_graph.h"

//*******************************************
// node ordering
//*******************************************

template <typename T>
concept IContractionOrder =
    requires(T t, std::array<std::tuple<int, bool>, 2> edges, int node, int from, int to, const std::vector<int>& in_neigbours, const std::vector<int>& out_neigbours) {
        {
            t.get_next()
        } -> std::same_as<int>;
        {
            t.get_hop_limit()
        } -> std::same_as<int>;
        {
            t.shortcut_added(from, to, edges)
        } -> std::same_as<void>;
        {
            t.node_contracted(node, in_neigbours, out_neigbours)
        } -> std::same_as<void>;
    };

//*******************************************
// PHAST Ordering (2*ED + CN + EC + 5*L)
//*******************************************

int _compute_phast_priority(int node, const CHPreprocGraph& graph, Flags<_FlagSH>& flags, std::priority_queue<pq_item>& heap, std::vector<int>& in_neigbours,
                            std::vector<int>& out_neigbours, const std::vector<int>& contracted_neighbours, const std::vector<char>& shortcut_edgecount, int hop_limit)
{
    _find_neighbours(graph, node, in_neigbours, out_neigbours);
    int edge_diff = -(in_neigbours.size() + out_neigbours.size());
    char edge_count = 0;
    for (int i = 0; i < in_neigbours.size(); i++) {
        int from = in_neigbours[i];
        flags.soft_reset();
        _run_local_search(node, from, graph, heap, flags, hop_limit);
        for (int j = 0; j < out_neigbours.size(); j++) {
            int to = out_neigbours[j];
            if (from == to) {
                continue;
            }
            auto [edges, shortcut_needed] = _get_shortcut(from, to, node, graph, flags);
            if (!shortcut_needed) {
                continue;
            }
            edge_diff += 2;
            // compute number of edges representing the shortcut (limited to 3)
            char ec = 0;
            if (!std::get<1>(edges[0])) {
                ec += 1;
            } else {
                ec += shortcut_edgecount[std::get<0>(edges[0])];
            }
            if (!std::get<1>(edges[1])) {
                ec += 1;
            } else {
                ec += shortcut_edgecount[std::get<0>(edges[1])];
            }
            if (ec > 3) {
                ec = 3;
            }
            edge_count += ec;
        }
    }

    return 2 * edge_diff + contracted_neighbours[node] + int(edge_count) + 5 * graph.getNodeLevel(node);
}

struct PHASTOrdering
{
    CHPreprocGraph& graph;
    Flags<_FlagSH> flags;
    std::priority_queue<pq_item> heap;
    std::vector<int> in_neigbours;
    std::vector<int> out_neigbours;

    std::vector<int> node_priorities;
    std::priority_queue<pq<int, int>> contraction_order;
    std::vector<int> contracted_neighbours;
    std::vector<char> shortcut_edgecount;
    int hop_limit;
    int node_count;
    int edge_count;
    int count = 0;

    PHASTOrdering(CHPreprocGraph& graph) : graph(graph)
    {
        this->contracted_neighbours.resize(graph.nodeCount());
        for (int i = 0; i < graph.nodeCount(); i++) {
            contracted_neighbours[i] = 0;
        }
        this->hop_limit = 5;
        this->node_count = graph.nodeCount();
        this->edge_count = graph.edgeCount() * 2;

        // initialize routing components
        this->flags = Flags<_FlagSH>(graph.nodeCount(), {.curr_length = 100000000});

        // compute node priorities
        std::cout << "computing priorities..." << std::endl;
        this->node_priorities.resize(graph.nodeCount());
        for (int i = 0; i < graph.nodeCount(); i++) {
            int prio = _compute_phast_priority(i, graph, this->flags, this->heap, this->in_neigbours, this->out_neigbours, this->contracted_neighbours, this->shortcut_edgecount,
                                               this->hop_limit);
            this->node_priorities[i] = prio;
            this->contraction_order.push({i, prio});
        }
    }

    int get_next()
    {
        while (!this->contraction_order.empty()) {
            auto [node_id, node_prio] = this->contraction_order.top();
            this->contraction_order.pop();
            if (this->graph.isContracted(node_id) || node_prio != this->node_priorities[node_id]) {
                continue;
            }
            this->count += 1;
            if (this->count % 1000 == 0) {
                int avg_degree = this->edge_count / this->node_count;
                std::cout << "	node :" << this->count << "/" << graph.nodeCount() << "	avg_degree:" << avg_degree << std::endl;
            }
            return node_id;
        }
        return -1;
    }

    int get_hop_limit() const { return hop_limit; }

    void shortcut_added(int from, int to, std::array<std::tuple<int, bool>, 2> edges)
    {
        // compute number of edges representing the shortcut (limited to 3)
        char ec = 0;
        if (!std::get<1>(edges[0])) {
            ec += 1;
        } else {
            ec += this->shortcut_edgecount[std::get<0>(edges[0])];
        }
        if (!std::get<1>(edges[1])) {
            ec += 1;
        } else {
            ec += this->shortcut_edgecount[std::get<0>(edges[1])];
        }
        if (ec > 3) {
            ec = 3;
        }
        this->shortcut_edgecount.push_back(ec);
        this->edge_count += 2;
    }

    void node_contracted(int node, const std::vector<int>& in_neigbours, const std::vector<int>& out_neigbours)
    {
        this->node_count -= 1;
        this->edge_count -= in_neigbours.size() + out_neigbours.size();
        if (this->node_count > 0) {
            int avg_degree = this->edge_count / this->node_count;
            if (this->hop_limit < 10 && avg_degree > 5) {
                this->hop_limit = 10;
            }
            if (this->hop_limit < 100 && avg_degree > 10) {
                this->hop_limit = 10;
            }
            if (this->hop_limit < 1000 && avg_degree > 20) {
                this->hop_limit = 1000;
            }
        }

        for (int i = 0; i < in_neigbours.size(); i++) {
            int nb = in_neigbours[i];
            this->contracted_neighbours[nb] += 1;
            int prio = _compute_phast_priority(nb, this->graph, this->flags, this->heap, this->in_neigbours, this->out_neigbours, this->contracted_neighbours,
                                               this->shortcut_edgecount, this->hop_limit);
            this->node_priorities[nb] = prio;
            this->contraction_order.push({nb, prio});
        }
        for (int i = 0; i < out_neigbours.size(); i++) {
            int nb = out_neigbours[i];
            this->contracted_neighbours[nb] += 1;
            int prio = _compute_phast_priority(nb, this->graph, this->flags, this->heap, this->in_neigbours, this->out_neigbours, this->contracted_neighbours,
                                               this->shortcut_edgecount, this->hop_limit);
            this->node_priorities[nb] = prio;
            this->contraction_order.push({nb, prio});
        }
    }
};

// Computes border nodes of graph.
static std::vector<bool> _get_is_border(CHPreprocGraph& graph, const Partition& partition)
{
    std::vector<bool> is_border_list(graph.nodeCount());

    for (int i = 0; i < graph.nodeCount(); i++) {
        int id = i;
        short tile = partition.get_node_tile(id);
        bool is_border = false;
        graph.forAdjacentEdges(id, Direction::BACKWARD, Adjacency::ADJACENT_ALL, [id, &is_border, &partition, tile](EdgeRef ref) {
            if (is_border) {
                return;
            }
            short other_tile = partition.get_node_tile(ref.other_id);
            if (tile != other_tile) {
                is_border = true;
            }
        });
        graph.forAdjacentEdges(id, Direction::FORWARD, Adjacency::ADJACENT_ALL, [id, &is_border, &partition, tile](EdgeRef ref) {
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

struct GSPHASTOrdering
{
    CHPreprocGraph& graph;
    const Partition& partition;
    std::vector<bool> is_border;
    Flags<_FlagSH> flags;
    std::priority_queue<pq_item> heap;
    std::vector<int> in_neigbours;
    std::vector<int> out_neigbours;

    std::vector<int> node_priorities;
    std::priority_queue<pq<int, int>> contraction_order;
    std::vector<int> contracted_neighbours;
    std::vector<char> shortcut_edgecount;
    int hop_limit;
    bool is_border_contraction = false;
    int node_count;
    int edge_count;
    int count = 0;

    GSPHASTOrdering(CHPreprocGraph& graph, const Partition& partition) : graph(graph), partition(partition)
    {
        this->is_border = _get_is_border(graph, partition);

        this->contracted_neighbours.resize(graph.nodeCount());
        for (int i = 0; i < graph.nodeCount(); i++) {
            contracted_neighbours[i] = 0;
        }
        this->hop_limit = 5;
        this->node_count = graph.nodeCount();
        this->edge_count = graph.edgeCount() * 2;

        // initialize routing components
        this->flags = Flags<_FlagSH>(graph.nodeCount(), {.curr_length = 100000000});

        // compute node priorities
        std::cout << "computing priorities..." << std::endl;
        this->node_priorities.resize(graph.nodeCount());
        for (int i = 0; i < graph.nodeCount(); i++) {
            if (this->is_border[i]) {
                this->node_priorities[i] = 1000000000;
                continue;
            }
            int prio = _compute_phast_priority(i, graph, this->flags, this->heap, this->in_neigbours, this->out_neigbours, this->contracted_neighbours, this->shortcut_edgecount,
                                               this->hop_limit);
            this->node_priorities[i] = prio;
            this->contraction_order.push({i, prio});
        }
    }

    int get_next()
    {
        while (true) {
            if (this->contraction_order.empty()) {
                if (this->is_border_contraction) {
                    return -1;
                }
                this->is_border_contraction = true;
                for (int i = 0; i < this->graph.nodeCount(); i++) {
                    if (!this->is_border[i]) {
                        continue;
                    }
                    int prio = _compute_phast_priority(i, graph, flags, heap, in_neigbours, out_neigbours, contracted_neighbours, shortcut_edgecount, 1000);
                    this->node_priorities[i] = prio;
                    this->contraction_order.push({i, prio});
                }
            }
            auto [node_id, node_prio] = this->contraction_order.top();
            this->contraction_order.pop();
            if (this->graph.isContracted(node_id) || node_prio != this->node_priorities[node_id]) {
                continue;
            }
            this->count += 1;
            if (this->count % 1000 == 0) {
                int avg_degree = this->edge_count / this->node_count;
                std::cout << "	node :" << this->count << "/" << graph.nodeCount() << "	avg_degree:" << avg_degree << std::endl;
            }
            return node_id;
        }
    }

    int get_hop_limit() const { return hop_limit; }

    void shortcut_added(int from, int to, std::array<std::tuple<int, bool>, 2> edges)
    {
        // compute number of edges representing the shortcut (limited to 3)
        char ec = 0;
        if (!std::get<1>(edges[0])) {
            ec += 1;
        } else {
            ec += this->shortcut_edgecount[std::get<0>(edges[0])];
        }
        if (!std::get<1>(edges[1])) {
            ec += 1;
        } else {
            ec += this->shortcut_edgecount[std::get<0>(edges[1])];
        }
        if (ec > 3) {
            ec = 3;
        }
        this->shortcut_edgecount.push_back(ec);
        this->edge_count += 2;
    }

    void node_contracted(int node, const std::vector<int>& in_neigbours, const std::vector<int>& out_neigbours)
    {
        this->node_count -= 1;
        this->edge_count -= in_neigbours.size() + out_neigbours.size();
        if (this->node_count > 0) {
            int avg_degree = this->edge_count / this->node_count;
            if (this->hop_limit < 10 && avg_degree > 5) {
                this->hop_limit = 10;
            }
            if (this->hop_limit < 100 && avg_degree > 10) {
                this->hop_limit = 10;
            }
            if (this->hop_limit < 1000 && avg_degree > 20) {
                this->hop_limit = 1000;
            }
        }

        for (int i = 0; i < in_neigbours.size(); i++) {
            int nb = in_neigbours[i];
            this->contracted_neighbours[nb] += 1;
            if (this->is_border[nb] && !this->is_border_contraction) {
                continue;
            }
            int prio = _compute_phast_priority(nb, this->graph, this->flags, this->heap, this->in_neigbours, this->out_neigbours, this->contracted_neighbours,
                                               this->shortcut_edgecount, this->hop_limit);
            this->node_priorities[nb] = prio;
            this->contraction_order.push({nb, prio});
        }
        for (int i = 0; i < out_neigbours.size(); i++) {
            int nb = out_neigbours[i];
            this->contracted_neighbours[nb] += 1;
            if (this->is_border[nb] && !this->is_border_contraction) {
                continue;
            }
            int prio = _compute_phast_priority(nb, this->graph, this->flags, this->heap, this->in_neigbours, this->out_neigbours, this->contracted_neighbours,
                                               this->shortcut_edgecount, this->hop_limit);
            this->node_priorities[nb] = prio;
            this->contraction_order.push({nb, prio});
        }
    }
};

struct PartialPHASTOrdering
{
    CHPreprocGraph& graph;
    const Partition& partition;
    std::vector<bool> is_border;
    Flags<_FlagSH> flags;
    std::priority_queue<pq_item> heap;
    std::vector<int> in_neigbours;
    std::vector<int> out_neigbours;

    std::vector<int> node_priorities;
    std::priority_queue<pq<int, int>> contraction_order;
    std::vector<int> contracted_neighbours;
    std::vector<char> shortcut_edgecount;
    int hop_limit;
    int node_count;
    int edge_count;
    int count = 0;

    PartialPHASTOrdering(CHPreprocGraph& graph, const Partition& partition) : graph(graph), partition(partition)
    {
        this->is_border = _get_is_border(graph, partition);

        this->contracted_neighbours.resize(graph.nodeCount());
        for (int i = 0; i < graph.nodeCount(); i++) {
            contracted_neighbours[i] = 0;
        }
        this->hop_limit = 5;
        this->node_count = graph.nodeCount();
        this->edge_count = graph.edgeCount() * 2;

        // initialize routing components
        this->flags = Flags<_FlagSH>(graph.nodeCount(), {.curr_length = 100000000});

        // compute node priorities
        std::cout << "computing priorities..." << std::endl;
        this->node_priorities.resize(graph.nodeCount());
        for (int i = 0; i < graph.nodeCount(); i++) {
            if (this->is_border[i]) {
                this->node_priorities[i] = 1000000000;
                continue;
            }
            int prio = _compute_phast_priority(i, graph, this->flags, this->heap, this->in_neigbours, this->out_neigbours, this->contracted_neighbours, this->shortcut_edgecount,
                                               this->hop_limit);
            this->node_priorities[i] = prio;
            this->contraction_order.push({i, prio});
        }
    }

    int get_next()
    {
        while (true) {
            if (this->contraction_order.empty()) {
                return -1;
            }
            auto [node_id, node_prio] = this->contraction_order.top();
            this->contraction_order.pop();
            if (this->graph.isContracted(node_id) || node_prio != this->node_priorities[node_id]) {
                continue;
            }
            this->count += 1;
            if (this->count % 1000 == 0) {
                int avg_degree = this->edge_count / this->node_count;
                std::cout << "	node :" << this->count << "/" << graph.nodeCount() << "	avg_degree:" << avg_degree << std::endl;
            }
            return node_id;
        }
    }

    int get_hop_limit() const { return hop_limit; }

    void shortcut_added(int from, int to, std::array<std::tuple<int, bool>, 2> edges)
    {
        // compute number of edges representing the shortcut (limited to 3)
        char ec = 0;
        if (!std::get<1>(edges[0])) {
            ec += 1;
        } else {
            ec += this->shortcut_edgecount[std::get<0>(edges[0])];
        }
        if (!std::get<1>(edges[1])) {
            ec += 1;
        } else {
            ec += this->shortcut_edgecount[std::get<0>(edges[1])];
        }
        if (ec > 3) {
            ec = 3;
        }
        this->shortcut_edgecount.push_back(ec);
        this->edge_count += 2;
    }

    void node_contracted(int node, const std::vector<int>& in_neigbours, const std::vector<int>& out_neigbours)
    {
        this->node_count -= 1;
        this->edge_count -= in_neigbours.size() + out_neigbours.size();
        if (this->node_count > 0) {
            int avg_degree = this->edge_count / this->node_count;
            if (this->hop_limit < 10 && avg_degree > 5) {
                this->hop_limit = 10;
            }
            if (this->hop_limit < 100 && avg_degree > 10) {
                this->hop_limit = 10;
            }
            if (this->hop_limit < 1000 && avg_degree > 20) {
                this->hop_limit = 1000;
            }
        }

        for (int i = 0; i < in_neigbours.size(); i++) {
            int nb = in_neigbours[i];
            this->contracted_neighbours[nb] += 1;
            if (this->is_border[nb]) {
                continue;
            }
            int prio = _compute_phast_priority(nb, this->graph, this->flags, this->heap, this->in_neigbours, this->out_neigbours, this->contracted_neighbours,
                                               this->shortcut_edgecount, this->hop_limit);
            this->node_priorities[nb] = prio;
            this->contraction_order.push({nb, prio});
        }
        for (int i = 0; i < out_neigbours.size(); i++) {
            int nb = out_neigbours[i];
            this->contracted_neighbours[nb] += 1;
            if (this->is_border[nb]) {
                continue;
            }
            int prio = _compute_phast_priority(nb, this->graph, this->flags, this->heap, this->in_neigbours, this->out_neigbours, this->contracted_neighbours,
                                               this->shortcut_edgecount, this->hop_limit);
            this->node_priorities[nb] = prio;
            this->contraction_order.push({nb, prio});
        }
    }
};

//*******************************************
// Simple Ordering
//*******************************************

int _compute_simple_priority(int node, const CHPreprocGraph& graph, std::vector<int>& in_neigbours, std::vector<int>& out_neigbours)
{
    _find_neighbours(graph, node, in_neigbours, out_neigbours);
    return in_neigbours.size() + out_neigbours.size();
}

struct SimpleOrdering
{
    CHPreprocGraph& graph;
    std::vector<int> in_neigbours;
    std::vector<int> out_neigbours;

    std::vector<int> node_priorities;
    std::priority_queue<pq<int, int>> contraction_order;
    int hop_limit;
    int node_count;
    int edge_count;
    int count = 0;

    SimpleOrdering(CHPreprocGraph& graph) : graph(graph)
    {
        this->hop_limit = 5;
        this->node_count = graph.nodeCount();
        this->edge_count = graph.edgeCount() * 2;

        // compute node priorities
        std::cout << "computing priorities..." << std::endl;
        this->node_priorities.resize(graph.nodeCount());
        for (int i = 0; i < graph.nodeCount(); i++) {
            int prio = _compute_simple_priority(i, graph, this->in_neigbours, this->out_neigbours);
            this->node_priorities[i] = prio;
            this->contraction_order.push({i, prio});
        }
    }

    int get_next()
    {
        while (!this->contraction_order.empty()) {
            auto [node_id, node_prio] = this->contraction_order.top();
            this->contraction_order.pop();
            if (this->graph.isContracted(node_id) || node_prio != this->node_priorities[node_id]) {
                continue;
            }
            this->count += 1;
            if (this->count % 1000 == 0) {
                int avg_degree = this->edge_count / this->node_count;
                std::cout << "	node :" << this->count << "/" << graph.nodeCount() << "	avg_degree:" << avg_degree << std::endl;
            }
            return node_id;
        }
        return -1;
    }

    int get_hop_limit() const { return hop_limit; }

    void shortcut_added(int from, int to, std::array<std::tuple<int, bool>, 2> edges) { this->edge_count += 2; }

    void node_contracted(int node, const std::vector<int>& in_neigbours, const std::vector<int>& out_neigbours)
    {
        this->node_count -= 1;
        this->edge_count -= in_neigbours.size() + out_neigbours.size();

        for (int i = 0; i < in_neigbours.size(); i++) {
            int nb = in_neigbours[i];
            int prio = _compute_simple_priority(nb, this->graph, this->in_neigbours, this->out_neigbours);
            this->node_priorities[nb] = prio;
            this->contraction_order.push({nb, prio});
        }
        for (int i = 0; i < out_neigbours.size(); i++) {
            int nb = out_neigbours[i];
            int prio = _compute_simple_priority(nb, this->graph, this->in_neigbours, this->out_neigbours);
            this->node_priorities[nb] = prio;
            this->contraction_order.push({nb, prio});
        }
    }
};
