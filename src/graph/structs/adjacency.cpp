
#include "./adjacency.h"

//*******************************************
// adjacency
//*******************************************

// return the node degree for given direction
short AdjacencyArray::getDegree(int node, Direction dir)
{
    auto ref = this->node_entries[node];
    if (dir == Direction::FORWARD) {
        return ref.fwd_count;
    } else {
        return ref.bwd_count;
    }
}

AdjacencyAccessor AdjacencyArray::getNeighbours(int node, Direction dir)
{
    _NodeEntry node_entry = this->node_entries[node];
    _EdgeEntry* edge_refs;
    int start;
    int end;
    if (dir == Direction::FORWARD) {
        edge_refs = &(this->fwd_edge_entries[node_entry.fwd_start]);
        start = 0;
        end = node_entry.fwd_count;
    }
    if (dir == Direction::BACKWARD) {
        edge_refs = &(this->bwd_edge_entries[node_entry.bwd_start]);
        start = 0;
        end = node_entry.bwd_count;
    }
    return AdjacencyAccessor(edge_refs, start, end);
}

// return the node degree for given direction
short AdjacencyList::getDegree(int node, Direction dir)
{
    auto ref = this->node_entries[node];
    if (dir == Direction::FORWARD) {
        return ref.fwd_edges.size();
    } else {
        return ref.bwd_edges.size();
    }
}

AdjacencyAccessor AdjacencyList::getNeighbours(int node, Direction dir)
{
    _DynamicNodeEntry& node_entry = this->node_entries[node];
    _EdgeEntry* edge_refs;
    int start;
    int end;
    if (dir == Direction::FORWARD) {
        if (node_entry.fwd_edges.empty()) {
            edge_refs = nullptr;
        } else {
            edge_refs = (&node_entry.fwd_edges[0]);
        }
        start = 0;
        end = node_entry.fwd_edges.size();
    }
    if (dir == Direction::BACKWARD) {
        if (node_entry.bwd_edges.empty()) {
            edge_refs = nullptr;
        } else {
            edge_refs = (&node_entry.bwd_edges[0]);
        }
        start = 0;
        end = node_entry.bwd_edges.size();
    }
    return AdjacencyAccessor(edge_refs, start, end);
}

void AdjacencyList::addEdgeEntries(int node_a, int node_b, int edge_id, char edge_typ)
{
    auto fwd_edges = this->node_entries[node_a].fwd_edges;
    fwd_edges.push_back(_EdgeEntry{edge_id, node_b, edge_typ});
    this->node_entries[node_a].fwd_edges = fwd_edges;
    auto bwd_edges = this->node_entries[node_b].bwd_edges;
    bwd_edges.push_back(_EdgeEntry{edge_id, node_a, edge_typ});
    this->node_entries[node_b].bwd_edges = bwd_edges;
}

// adds forward entry to adjacency
//
// refers to edge between node_a and node_b, entry will be added at node_a
void AdjacencyList::addFWDEntry(int node_a, int node_b, int edge_id, char edge_typ)
{
    auto fwd_edges = this->node_entries[node_a].fwd_edges;
    fwd_edges.push_back(_EdgeEntry{edge_id, node_b, edge_typ});
    this->node_entries[node_a].fwd_edges = fwd_edges;
}

// adds backward entry to adjacency
//
// refers to edge between node_a and node_b, entry will be added at node_b
void AdjacencyList::addBWDEntry(int node_a, int node_b, int edge_id, char edge_typ)
{
    auto bwd_edges = this->node_entries[node_b].bwd_edges;
    bwd_edges.push_back(_EdgeEntry{edge_id, node_a, edge_typ});
    this->node_entries[node_b].bwd_edges = bwd_edges;
}

//*******************************************
// adjacency accessor
//*******************************************

bool AdjacencyAccessor::next()
{
    if (this->state == this->end) {
        return false;
    }
    this->state += 1;
    return true;
}
int AdjacencyAccessor::getEdgeID()
{
    return this->edge_refs[this->state - 1].edge_id;
}
int AdjacencyAccessor::getOtherID()
{
    return this->edge_refs[this->state - 1].other_id;
}
std::array<char, 8> AdjacencyAccessor::getData()
{
    return this->edge_refs[this->state - 1].data;
}

//*******************************************
// build adjacency
//*******************************************

AdjacencyArray build_adjacency_array(std::vector<Node>& nodes, std::vector<Edge>& edges)
{
    AdjacencyList adj_list(nodes.size());
    for (int i = 0; i < edges.size(); i++) {
        auto edge = edges[i];
        adj_list.addFWDEntry(edge.nodeA, edge.nodeB, i, 0);
        adj_list.addBWDEntry(edge.nodeA, edge.nodeB, i, 0);
    }

    return build_adjacency_array(adj_list);
}

AdjacencyArray build_adjacency_array(AdjacencyList& adj_list)
{
    std::vector<_NodeEntry> node_entries;
    std::vector<_EdgeEntry> fwd_entries;
    std::vector<_EdgeEntry> bwd_entries;

    for (int i = 0; i < adj_list.node_entries.size(); i++) {
        auto& node_entry = adj_list.node_entries[i];
        int fwd_start = fwd_entries.size();
        short fwd_count = 0;
        for (auto edge_entry : node_entry.fwd_edges) {
            fwd_entries.push_back(edge_entry);
            fwd_count += 1;
        }
        int bwd_start = bwd_entries.size();
        short bwd_count = 0;
        for (auto edge_entry : node_entry.bwd_edges) {
            bwd_entries.push_back(edge_entry);
            bwd_count += 1;
        }
        _NodeEntry new_entry = {
            .fwd_start = fwd_start,
            .fwd_count = fwd_count,
            .bwd_start = bwd_start,
            .bwd_count = bwd_count,
        };
        node_entries.push_back(new_entry);
    }

    return AdjacencyArray(node_entries, fwd_entries, bwd_entries);
}
