#include <vector>

#include "./graph_base_mod.h"

std::shared_ptr<GraphBase> _remove_nodes(const GraphBase& base, const std::vector<int>& nodes)
{
    std::vector<bool> remove(base.nodeCount(), false);
    for (int node : nodes) {
        remove[node] = true;
    }

    std::vector<Node> new_nodes;
    std::vector<int> mapping(base.nodeCount());
    int id = 0;
    for (int i = 0; i < base.nodeCount(); i++) {
        if (remove[i]) {
            mapping[i] = -1;
            continue;
        }
        new_nodes.push_back(base.getNode(i));
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
        });
    }

    return new_graph_base(new_nodes, new_edges);
}

// reorders nodes,
// mapping: old id -> new id
std::shared_ptr<GraphBase> _reorder_nodes(const GraphBase& base, const std::vector<int>& mapping)
{
    std::vector<Node> new_nodes(base.nodes.size());
    for (int i = 0; i < base.nodeCount(); i++) {
        int m_id = mapping[i];
        new_nodes[m_id] = base.getNode(i);
    }
    std::vector<Edge> new_edges;
    for (int i = 0; i < base.edgeCount(); i++) {
        auto edge = base.getEdge(i);
        new_edges.push_back({
            .nodeA = mapping[edge.nodeA],
            .nodeB = mapping[edge.nodeB],
        });
    }

    return new_graph_base(new_nodes, new_edges);
}
