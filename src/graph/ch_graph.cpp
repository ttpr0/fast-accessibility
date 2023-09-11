
#include "./ch_graph.h"

//*******************************************
// base-graph
//******************************************

CHGraph::CHGraph(GraphStore store, TopologyStore topology, std::vector<int> weights, CHStore ch_store, TopologyStore ch_topology)
    : store(store), topology(topology), edge_weights(weights), ch_store(ch_store), ch_topology(ch_topology)
{
    std::vector<CHEdge> fwd_down_edges;
    std::vector<CHEdge> bwd_down_edges;

    auto explorer = this->getGraphExplorer();
    for (int i = 0; i < this->nodeCount(); i++) {
        int this_id = i;
        int count = 0;
        explorer->forAdjacentEdges(this_id, Direction::FORWARD, Adjacency::ADJACENT_DOWNWARDS, [&explorer, &fwd_down_edges, &count, &this_id](EdgeRef ref) {
            int other_id = ref.other_id;
            int weight = explorer->getEdgeWeight(ref);
            fwd_down_edges.push_back(CHEdge{this_id, other_id, weight, 1});
            count += 1;
        });
        for (int j = fwd_down_edges.size() - count; j < fwd_down_edges.size(); j++) {
            auto ch_edge = fwd_down_edges[j];
            ch_edge.count = count;
            fwd_down_edges[j] = ch_edge;
        }

        count = 0;
        explorer->forAdjacentEdges(this_id, Direction::BACKWARD, Adjacency::ADJACENT_DOWNWARDS, [&explorer, &bwd_down_edges, &count, &this_id](EdgeRef ref) {
            int other_id = ref.other_id;
            int weight = explorer->getEdgeWeight(ref);
            bwd_down_edges.push_back(CHEdge{this_id, other_id, weight, 1});
            count += 1;
        });
        for (int j = bwd_down_edges.size() - count; j < bwd_down_edges.size(); j++) {
            auto ch_edge = bwd_down_edges[j];
            ch_edge.count = count;
            bwd_down_edges[j] = ch_edge;
        }
    }

    this->fwd_down_edges = fwd_down_edges;
    this->bwd_down_edges = bwd_down_edges;
}

std::unique_ptr<IGraphExplorer> CHGraph::getGraphExplorer()
{
    return std::make_unique<CHGraphExplorer>(this, this->topology.getAccessor(), this->ch_topology.getAccessor(), this->edge_weights.data(), this->ch_store.sh_weights.data(),
                                             this->ch_store.node_levels.data());
}
std::unique_ptr<IGraphIndex> CHGraph::getIndex()
{
    return std::make_unique<BaseGraphIndex>(this->nodeCount(), this->store.node_geoms.data());
}
int CHGraph::nodeCount()
{
    return this->store.nodeCount();
}
int CHGraph::edgeCount()
{
    return this->store.edgeCount();
}
Node CHGraph::getNode(int node)
{
    return this->store.getNode(node);
}
Edge CHGraph::getEdge(int edge)
{
    return this->store.getEdge(edge);
}
Coord CHGraph::getNodeGeom(int node)
{
    return this->store.getNodeGeom(node);
}

short CHGraph::getNodeLevel(int node)
{
    return this->ch_store.getNodeLevel(node);
}
int CHGraph::shortcutCount()
{
    return this->ch_store.shortcutCount();
}
CHShortcut CHGraph::getShortcut(int shortcut)
{
    return this->ch_store.getShortcut(shortcut);
}
std::tuple<CHEdge*, int> CHGraph::getDownEdges(Direction dir)
{
    if (dir == Direction::FORWARD) {
        return std::make_tuple(this->fwd_down_edges.data(), this->fwd_down_edges.size());
    } else {
        return std::make_tuple(this->bwd_down_edges.data(), this->bwd_down_edges.size());
    }
}

//*******************************************
// ch-graph explorer
//******************************************

void CHGraphExplorer::forAdjacentEdges(int node, Direction dir, Adjacency typ, std::function<void(EdgeRef)> func)
{
    if (typ == Adjacency::ADJACENT_ALL) {
        this->accessor.setBaseNode(node, dir);
        this->sh_accessor.setBaseNode(node, dir);
        while (this->accessor.next()) {
            int edge_id = this->accessor.getEdgeID();
            int other_id = this->accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 0});
        }
        while (this->sh_accessor.next()) {
            int edge_id = this->sh_accessor.getEdgeID();
            int other_id = this->sh_accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 100});
        }
    } else if (typ == Adjacency::ADJACENT_EDGES) {
        this->accessor.setBaseNode(node, dir);
        while (this->accessor.next()) {
            int edge_id = this->accessor.getEdgeID();
            int other_id = this->accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 0});
        }
    } else if (typ == Adjacency::ADJACENT_SHORTCUTS) {
        this->sh_accessor.setBaseNode(node, dir);
        while (this->sh_accessor.next()) {
            int edge_id = this->sh_accessor.getEdgeID();
            int other_id = this->sh_accessor.getOtherID();
            func(EdgeRef{edge_id, other_id, 0});
        }
    } else if (typ == Adjacency::ADJACENT_UPWARDS) {
        this->accessor.setBaseNode(node, dir);
        this->sh_accessor.setBaseNode(node, dir);
        while (this->accessor.next()) {
            int other_id = this->accessor.getOtherID();
            if (this->node_levels[node] >= this->node_levels[other_id]) {
                continue;
            }
            int edge_id = this->accessor.getEdgeID();
            func(EdgeRef{edge_id, other_id, 0});
        }
        while (this->sh_accessor.next()) {
            int other_id = this->sh_accessor.getOtherID();
            if (this->node_levels[node] >= this->node_levels[other_id]) {
                continue;
            }
            int edge_id = this->sh_accessor.getEdgeID();
            func(EdgeRef{edge_id, other_id, 100});
        }
    } else if (typ == Adjacency::ADJACENT_DOWNWARDS) {
        this->accessor.setBaseNode(node, dir);
        this->sh_accessor.setBaseNode(node, dir);
        while (this->accessor.next()) {
            int other_id = this->accessor.getOtherID();
            if (this->node_levels[node] <= this->node_levels[other_id]) {
                continue;
            }
            int edge_id = this->accessor.getEdgeID();
            func(EdgeRef{edge_id, other_id, 0});
        }
        while (this->sh_accessor.next()) {
            int other_id = this->sh_accessor.getOtherID();
            if (this->node_levels[node] <= this->node_levels[other_id]) {
                continue;
            }
            int edge_id = this->sh_accessor.getEdgeID();
            func(EdgeRef{edge_id, other_id, 100});
        }
    } else {
        throw 1;
    }
    return;
}
int CHGraphExplorer::getEdgeWeight(EdgeRef edge)
{
    if (edge.isCHShortcut()) {
        return this->sh_weights[edge.edge_id];
    } else {
        return this->edge_weights[edge.edge_id];
    }
}
int CHGraphExplorer::getTurnCost(EdgeRef from, int via, EdgeRef to)
{
    return 0;
}
int CHGraphExplorer::getOtherNode(EdgeRef edge, int node)
{
    if (edge.isShortcut()) {
        auto e = this->graph->getShortcut(edge.edge_id);
        if (node == e.nodeA) {
            return e.nodeB;
        }
        if (node == e.nodeB) {
            return e.nodeA;
        }
        return -1;
    } else {
        auto e = this->graph->getEdge(edge.edge_id);
        if (node == e.nodeA) {
            return e.nodeB;
        }
        if (node == e.nodeB) {
            return e.nodeA;
        }
        return -1;
    }
}
