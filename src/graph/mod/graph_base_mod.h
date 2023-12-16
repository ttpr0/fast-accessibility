#pragma once

#include <memory>
#include <vector>

#include "../comps/graph_base.h"

static std::shared_ptr<GraphBase> _remove_nodes(const GraphBase& base, const std::vector<int>& nodes)
{
    std::vector<bool> remove(base.nodeCount(), false);
    for (int node : nodes) {
        remove[node] = true;
    }

    std::vector<Node> new_nodes;
    std::vector<Coord> new_node_geoms;
    std::vector<int> mapping(base.nodeCount());
    int id = 0;
    for (int i = 0; i < base.nodeCount(); i++) {
        if (remove[i]) {
            mapping[i] = -1;
            continue;
        }
        new_nodes.push_back(base.getNode(i));
        new_node_geoms.push_back(base.getNodeGeom(i));
        mapping[i] = id;
        id += 1;
    }
    std::vector<Edge> new_edges;
    for (int i = 0; i < base.edgeCount(); i++) {
        auto edge = base.getEdge(i);
        if (remove[edge.nodeA] || remove[edge.nodeB]) {
            continue;
        }
        new_edges.push_back({
            .nodeA = mapping[edge.nodeA],
            .nodeB = mapping[edge.nodeB],
            .type = edge.type,
            .length = edge.length,
            .maxspeed = edge.maxspeed,
            .oneway = edge.oneway,
        });
    }

    return new_graph_base(new_nodes, new_edges, new_node_geoms);
}

// reorders nodes,
// mapping: old id -> new id
static std::shared_ptr<GraphBase> _reorder_nodes(const GraphBase& base, const std::vector<int>& mapping)
{
    std::vector<Node> new_nodes(base.nodes.size());
    std::vector<Coord> new_node_geoms(base.nodes.size());
    for (int i = 0; i < base.nodeCount(); i++) {
        int m_id = mapping[i];
        new_nodes[m_id] = base.getNode(i);
        new_node_geoms[m_id] = base.getNodeGeom(i);
    }
    std::vector<Edge> new_edges;
    for (int i = 0; i < base.edgeCount(); i++) {
        auto edge = base.getEdge(i);
        new_edges.push_back({
            .nodeA = mapping[edge.nodeA],
            .nodeB = mapping[edge.nodeB],
            .type = edge.type,
            .length = edge.length,
            .maxspeed = edge.maxspeed,
            .oneway = edge.oneway,
        });
    }

    return new_graph_base(new_nodes, new_edges, new_node_geoms);
}
