
#include "./ch_graph.h"

//*******************************************
// base-graph
//******************************************

CHGraph2::CHGraph2(GraphStore store, TopologyStore topology, std::vector<int> weights, CHStore ch_store, TopologyStore ch_topology, std::vector<short> node_tiles)
    : ch_graph(store, topology, weights, ch_store, ch_topology)
{
    auto explorer = this->ch_graph.getGraphExplorer();

    // get border nodes
    std::vector<bool> is_border(this->nodeCount());
    for (int i = 0; i < this->nodeCount(); i++) {
        explorer->forAdjacentEdges(i, FORWARD, ADJACENT_ALL, [&](EdgeRef ref) {
            if (node_tiles[i] != node_tiles[ref.other_id]) {
                is_border[i] = true;
            }
        });
        explorer->forAdjacentEdges(i, BACKWARD, ADJACENT_ALL, [&](EdgeRef ref) {
            if (node_tiles[i] != node_tiles[ref.other_id]) {
                is_border[i] = true;
            }
        });
    }

    // initialize down edges lists
    std::vector<CHEdge4> fwd_down_edges;
    std::vector<CHEdge4> bwd_down_edges;
    int border_count = 0;

    // add overlay down-edges
    fwd_down_edges.push_back(CHEdge4{0, 0, 0, 0, true});
    bwd_down_edges.push_back(CHEdge4{0, 0, 0, 0, true});
    std::unordered_map<short, std::vector<CHEdge4>> fwd_other_edges;
    std::unordered_map<short, std::vector<CHEdge4>> bwd_other_edges;
    for (int i = 0; i < this->nodeCount(); i++) {
        int this_id = i;
        short this_tile = node_tiles[this_id];
        if (!is_border[this_id]) {
            border_count = i + 1;
            break;
        }
        explorer->forAdjacentEdges(this_id, Direction::FORWARD, Adjacency::ADJACENT_DOWNWARDS, [&](EdgeRef ref) {
            int other_id = ref.other_id;
            short other_tile = node_tiles[other_id];
            CHEdge4 edge = {this_id, other_id, explorer->getEdgeWeight(ref), other_tile, false};
            if (!is_border[other_id]) {
                std::vector<CHEdge4> edges;
                if (fwd_other_edges.contains(this_tile)) {
                    edges = fwd_other_edges[this_tile];
                }
                edges.push_back(edge);
                fwd_other_edges[this_tile] = edges;
            } else {
                fwd_down_edges.push_back(edge);
            }
        });
        explorer->forAdjacentEdges(this_id, Direction::BACKWARD, Adjacency::ADJACENT_DOWNWARDS, [&](EdgeRef ref) {
            int other_id = ref.other_id;
            short other_tile = node_tiles[other_id];
            CHEdge4 edge = {this_id, other_id, explorer->getEdgeWeight(ref), other_tile, false};
            if (!is_border[other_id]) {
                std::vector<CHEdge4> edges;
                if (bwd_other_edges.contains(this_tile)) {
                    edges = bwd_other_edges[this_tile];
                }
                edges.push_back(edge);
                bwd_other_edges[this_tile] = edges;
            } else {
                bwd_down_edges.push_back(edge);
            }
        });
    }
    // add other down edges
    short curr_tile = -1;
    for (int i = border_count; i < this->nodeCount(); i++) {
        int this_id = i;
        short this_tile = node_tiles[this_id];
        if (this_tile != curr_tile) {
            fwd_down_edges.push_back(CHEdge4{0, 0, 0, this_tile, true});
            bwd_down_edges.push_back(CHEdge4{0, 0, 0, this_tile, true});
            curr_tile = this_tile;
            if (fwd_other_edges.contains(this_tile)) {
                auto& edges = fwd_other_edges[this_tile];
                for (auto edge : edges) {
                    fwd_down_edges.push_back(edge);
                }
            }
            if (bwd_other_edges.contains(this_tile)) {
                auto& edges = bwd_other_edges[this_tile];
                for (auto edge : edges) {
                    bwd_down_edges.push_back(edge);
                }
            }
        }
        explorer->forAdjacentEdges(this_id, Direction::FORWARD, Adjacency::ADJACENT_DOWNWARDS, [&](EdgeRef ref) {
            int other_id = ref.other_id;
            fwd_down_edges.push_back(CHEdge4{this_id, other_id, explorer->getEdgeWeight(ref), 0, false});
        });
        explorer->forAdjacentEdges(this_id, Direction::BACKWARD, Adjacency::ADJACENT_DOWNWARDS, [&](EdgeRef ref) {
            int other_id = ref.other_id;
            bwd_down_edges.push_back(CHEdge4{this_id, other_id, explorer->getEdgeWeight(ref), 0, false});
        });
    }

    // set count in dummy edges
    int fwd_id = 0;
    int fwd_count = 0;
    for (int i = 0; i < fwd_down_edges.size(); i++) {
        CHEdge4 edge = fwd_down_edges[i];
        if (edge.is_dummy) {
            // set count in previous dummy
            fwd_down_edges[fwd_id].to = fwd_count;
            // reset count
            fwd_id = i;
            fwd_count = 0;
            continue;
        }
        fwd_count += 1;
    }
    fwd_down_edges[fwd_id].to = fwd_count;
    int bwd_id = 0;
    int bwd_count = 0;
    for (int i = 0; i < bwd_down_edges.size(); i++) {
        CHEdge4 edge = bwd_down_edges[i];
        if (edge.is_dummy) {
            // set count in previous dummy
            bwd_down_edges[bwd_id].to = bwd_count;
            // reset count
            bwd_id = i;
            bwd_count = 0;
            continue;
        }
        bwd_count += 1;
    }
    bwd_down_edges[bwd_id].to = bwd_count;

    // add to graph
    this->node_tiles = std::move(node_tiles);
    this->fwd_down_edges = std::move(fwd_down_edges);
    this->bwd_down_edges = std::move(bwd_down_edges);
}

std::unique_ptr<IGraphExplorer> CHGraph2::getGraphExplorer()
{
    return std::move(this->ch_graph.getGraphExplorer());
}
IGraphIndex& CHGraph2::getIndex()
{
    return *this->ch_graph.index;
}
int CHGraph2::nodeCount()
{
    return this->ch_graph.nodeCount();
}
int CHGraph2::edgeCount()
{
    return this->ch_graph.edgeCount();
}
Node CHGraph2::getNode(int node)
{
    return this->ch_graph.getNode(node);
}
Edge CHGraph2::getEdge(int edge)
{
    return this->ch_graph.getEdge(edge);
}
Coord CHGraph2::getNodeGeom(int node)
{
    return this->ch_graph.getNodeGeom(node);
}

short CHGraph2::getNodeLevel(int node)
{
    return this->ch_graph.getNodeLevel(node);
}
int CHGraph2::shortcutCount()
{
    return this->ch_graph.shortcutCount();
}
CHShortcut CHGraph2::getShortcut(int shortcut)
{
    return this->ch_graph.getShortcut(shortcut);
}
const std::vector<CHEdge>& CHGraph2::getDownEdges(Direction dir)
{
    return this->ch_graph.getDownEdges(dir);
}

short CHGraph2::getNodeTile(int node)
{
    return this->node_tiles[node];
}
int CHGraph2::tileCount()
{
    short max = 0;
    for (int i = 0; i < this->node_tiles.size(); i++) {
        short tile = this->node_tiles[i];
        if (tile > max) {
            max = tile;
        }
    }
    return max + 1;
}
const std::vector<CHEdge4>& CHGraph2::getDownEdges4(Direction dir)
{
    if (dir == Direction::FORWARD) {
        return this->fwd_down_edges;
    } else {
        return this->bwd_down_edges;
    }
}