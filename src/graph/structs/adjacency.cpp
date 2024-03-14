
#include "./adjacency.h"

//*******************************************
// adjacency
//*******************************************

// return the node degree for given direction
short AdjacencyArray::getDegree(int node, Direction dir) const
{
    if (dir == Direction::FORWARD) {
        return this->fwd_entries.getCount(node);
    } else {
        return this->bwd_entries.getCount(node);
    }
}

AdjacencyAccessor AdjacencyArray::getNeighbours(int node, Direction dir) const
{
    if (dir == Direction::FORWARD) {
        auto accessor = this->fwd_entries.getValues(node);
        return AdjacencyAccessor{accessor};
    } else {
        auto accessor = this->bwd_entries.getValues(node);
        return AdjacencyAccessor{accessor};
    }
}

// return the node degree for given direction
short AdjacencyList::getDegree(int node, Direction dir) const
{
    if (dir == Direction::FORWARD) {
        return this->fwd_entries.getCount(node);
    } else {
        return this->bwd_entries.getCount(node);
    }
}

AdjacencyAccessor AdjacencyList::getNeighbours(int node, Direction dir) const
{
    if (dir == Direction::FORWARD) {
        auto accessor = this->fwd_entries.getValues(node);
        return AdjacencyAccessor{accessor};
    } else {
        auto accessor = this->bwd_entries.getValues(node);
        return AdjacencyAccessor{accessor};
    }
}

void AdjacencyList::addEdgeEntries(int node_a, int node_b, int edge_id, char edge_typ)
{
    this->fwd_entries.addValue(node_a, _EdgeEntry{edge_id, node_b, edge_typ});
    this->bwd_entries.addValue(node_b, _EdgeEntry{edge_id, node_a, edge_typ});
}

// adds forward entry to adjacency
//
// refers to edge between node_a and node_b, entry will be added at node_a
void AdjacencyList::addFWDEntry(int node_a, int node_b, int edge_id, char edge_typ)
{
    this->fwd_entries.addValue(node_a, _EdgeEntry{edge_id, node_b, edge_typ});
}

// adds backward entry to adjacency
//
// refers to edge between node_a and node_b, entry will be added at node_b
void AdjacencyList::addBWDEntry(int node_a, int node_b, int edge_id, char edge_typ)
{
    this->bwd_entries.addValue(node_b, _EdgeEntry{edge_id, node_a, edge_typ});
}

//*******************************************
// adjacency accessor
//*******************************************

bool AdjacencyAccessor::next()
{
    return this->state.next();
}
int AdjacencyAccessor::getEdgeID()
{
    return this->state.getValue().edge_id;
}
int AdjacencyAccessor::getOtherID()
{
    return this->state.getValue().other_id;
}
std::array<char, 8> AdjacencyAccessor::getData()
{
    return this->state.getValue().data;
}

//*******************************************
// build adjacency
//*******************************************

AdjacencyArray build_adjacency_array(std::vector<Node>& nodes, std::vector<Edge>& edges)
{
    AdjacencyList adj_list(nodes.size());
    for (int i = 0; i < edges.size(); i++) {
        auto& edge = edges[i];
        adj_list.addFWDEntry(edge.nodeA, edge.nodeB, i, 0);
        adj_list.addBWDEntry(edge.nodeA, edge.nodeB, i, 0);
    }

    return build_adjacency_array(adj_list);
}

AdjacencyArray build_adjacency_array(AdjacencyList& adj_list)
{
    return AdjacencyArray(adj_list.fwd_entries.toStatic(), adj_list.bwd_entries.toStatic());
}
