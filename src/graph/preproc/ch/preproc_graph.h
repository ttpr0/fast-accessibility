#pragma once

#include <math.h>
#include <array>
#include <memory>
#include <tuple>
#include <vector>

#include "../../base_graph.h"
#include "../../comps/graph_base.h"
#include "../../graph.h"
#include "../../speed_ups/ch_data.h"
#include "../../speed_ups/ch_index.h"
#include "../../speed_ups/partition.h"
#include "../../structs/adjacency.h"

//*******************************************
// preprocessing graph
//*******************************************

class CHPreprocGraph
{
public:
    std::shared_ptr<GraphBase> base;
    std::shared_ptr<Weighting> weights;

    std::vector<short> node_levels;
    std::vector<Shortcut> shortcuts;
    AdjacencyList ch_adjacency;

    CHPreprocGraph(std::shared_ptr<GraphBase> base, std::shared_ptr<Weighting> weights)
        : base(base), weights(weights), node_levels(base->nodeCount()), ch_adjacency(base->nodeCount())
    {}

    int nodeCount() { return this->base->nodeCount(); }
    int edgeCount() { return this->base->edgeCount(); }
    int shortcutCount() { return this->shortcuts.size(); }
    Node getNode(int node) { return this->base->getNode(node); }
    Edge getEdge(int edge) { return this->base->getEdge(edge); }
    Shortcut getShortcut(int shc) { return this->shortcuts[shc]; }

    int getWeight(int id, bool is_shortcut)
    {
        if (is_shortcut) {
            auto shc = this->getShortcut(id);
            return shc.weight;
        } else {
            return this->weights->get_edge_weight(id);
        }
    }
    short getNodeLevel(int id) { return this->node_levels[id]; }
    void setNodeLevel(int id, short level) { this->node_levels[id] = level; }
    void AddShortcut(int node_a, int node_b, std::array<std::tuple<int, bool>, 2> edges)
    {
        if (node_a == node_b) {
            return;
        }

        int weight = 0;
        weight += this->getWeight(std::get<0>(edges[0]), std::get<1>(edges[0]));
        weight += this->getWeight(std::get<0>(edges[1]), std::get<1>(edges[1]));
        Shortcut shc = {
            .from = node_a,
            .to = node_b,
            .weight = weight,
        };
        int shc_id = this->shortcuts.size();
        this->shortcuts.push_back(shc);

        this->ch_adjacency.addEdgeEntries(node_a, node_b, shc_id, 100);
    }

    void forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> callback)
    {
        auto accessor = this->base->adjacency.getNeighbours(node, dir);
        auto sh_accessor = this->ch_adjacency.getNeighbours(node, dir);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            callback({
                .edge_id = edge_id,
                .other_id = other_id,
                .type = 0,
            });
        }
        while (sh_accessor.next()) {
            int edge_id = sh_accessor.getEdgeID();
            int other_id = sh_accessor.getOtherID();
            callback({
                .edge_id = edge_id,
                .other_id = other_id,
                .type = 100,
            });
        }
    }
    int getEdgeWeight(EdgeRef ref) { return this->getWeight(ref.edge_id, ref.isCHShortcut()); }
    int getTurnCost(EdgeRef from, int via, EdgeRef to) { return 0; }
    int getOtherNode(int edge_id, int node, bool is_shortcut)
    {
        if (is_shortcut) {
            auto e = this->getShortcut(edge_id);
            if (node == e.from) {
                return e.to;
            }
            if (node == e.to) {
                return e.from;
            }
            return -1;
        } else {
            auto e = this->getEdge(edge_id);
            if (node == e.nodeA) {
                return e.nodeB;
            }
            if (node == e.nodeB) {
                return e.nodeA;
            }
            return -1;
        }
    }
    int getWeightBetween(int from, int to)
    {
        auto accessor = this->base->adjacency.getNeighbours(from, Direction::FORWARD);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            if (other_id == to) {
                return this->getWeight(edge_id, false);
            }
        }
        auto ch_accessor = this->ch_adjacency.getNeighbours(from, Direction::FORWARD);
        while (ch_accessor.next()) {
            int edge_id = ch_accessor.getEdgeID();
            int other_id = ch_accessor.getOtherID();
            if (other_id == to) {
                return this->getWeight(edge_id, false);
            }
        }
        return -1;
    }
    std::tuple<EdgeRef, bool> getEdgeBetween(int from, int to)
    {
        auto accessor = this->base->adjacency.getNeighbours(from, Direction::FORWARD);
        while (accessor.next()) {
            int edge_id = accessor.getEdgeID();
            int other_id = accessor.getOtherID();
            if (other_id == to) {
                return std::make_tuple(EdgeRef(edge_id, other_id, 0), true);
            }
        }
        auto ch_accessor = this->ch_adjacency.getNeighbours(from, Direction::FORWARD);
        while (ch_accessor.next()) {
            int edge_id = ch_accessor.getEdgeID();
            int other_id = ch_accessor.getOtherID();
            if (other_id == to) {
                return std::make_tuple(EdgeRef(edge_id, other_id, 100), true);
            }
        }
        return std::make_tuple(EdgeRef(), false);
    }

    // Invalidates the graph.
    // Use this only after ch-preprocessing is finished.
    std::shared_ptr<CHData> getCHData()
    {
        AdjacencyArray adjacency = build_adjacency_array(this->ch_adjacency);
        return std::make_shared<CHData>(std::move(this->shortcuts), std::move(adjacency), std::move(this->node_levels));
    }
};
