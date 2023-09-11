
#include "./topology_storage.h"

//*******************************************
// topology store
//*******************************************

// return the node degree for given direction
short TopologyStore::getDegree(int node, Direction dir) {
    auto ref = this->node_entries[node];
    if (dir == Direction::FORWARD) {
        return ref.fwd_count;
    }
    else {
        return ref.bwd_count;
    }
}

TopologyAccessor TopologyStore::getAccessor() {
    return TopologyAccessor(this);
}

// return the node degree for given direction
short TypedTopologyStore::getDegree(int node, Direction dir) {
    auto ref = this->node_entries[node];
    if (dir == Direction::FORWARD) {
        return ref.fwd_count;
    }
    else {
        return ref.bwd_count;
    }
}

TypedTopologyAccessor TypedTopologyStore::getAccessor() {
    return TypedTopologyAccessor(this);
}


// return the node degree for given direction
short DynamicTopologyStore::getDegree(int node, Direction dir) {
    auto ref = this->node_entries[node];
    if (dir == Direction::FORWARD) {
        return ref.fwd_edges.size();
    }
    else {
        return ref.bwd_edges.size();
    }
}

void DynamicTopologyStore::addEdgeEntries(int node_a, int node_b, int edge_id, char edge_typ) {
    auto fwd_edges = this->node_entries[node_a].fwd_edges;
    fwd_edges.push_back(_TypedEdgeEntry{ edge_id, node_b, edge_typ });
    this->node_entries[node_a].fwd_edges = fwd_edges;
    auto bwd_edges = this->node_entries[node_b].bwd_edges;
    bwd_edges.push_back(_TypedEdgeEntry{ edge_id, node_a, edge_typ });
    this->node_entries[node_b].bwd_edges = bwd_edges;
}

// adds forward entry to adjacency
//
// refers to edge between node_a and node_b, entry will be added at node_a
void DynamicTopologyStore::addFWDEntry(int node_a, int node_b, int edge_id, char edge_typ) {
    auto fwd_edges = this->node_entries[node_a].fwd_edges;
    fwd_edges.push_back(_TypedEdgeEntry{ edge_id, node_b, edge_typ });
    this->node_entries[node_a].fwd_edges = fwd_edges;
}

// adds backward entry to adjacency
//
// refers to edge between node_a and node_b, entry will be added at node_b
void DynamicTopologyStore::addBWDEntry(int node_a, int node_b, int edge_id, char edge_typ) {
    auto bwd_edges = this->node_entries[node_b].bwd_edges;
    bwd_edges.push_back(_TypedEdgeEntry{ edge_id, node_a, edge_typ });
    this->node_entries[node_b].bwd_edges = bwd_edges;
}

DynamicTopologyAccessor DynamicTopologyStore::getAccessor() {
    return DynamicTopologyAccessor(this);
}

//*******************************************
// topology accessor
//*******************************************

void TopologyAccessor::setBaseNode(int node, Direction dir) {
    auto ref = this->topology->node_entries[node];
    if (dir == Direction::FORWARD) {
        this->state = ref.fwd_start;
        this->end = ref.fwd_start + ref.fwd_count;
        this->edge_refs = this->topology->fwd_edge_entries.data();
    }
    else {
        this->state = ref.bwd_start;
        this->end = ref.bwd_start + ref.bwd_count;
        this->edge_refs = this->topology->bwd_edge_entries.data();
    }
}
bool TopologyAccessor::next() {
    if (this->state == this->end) {
        return false;
    }
    auto ref = this->edge_refs[this->state];
    this->curr_edge_id = ref.edge_id;
    this->curr_other_id = ref.other_id;
    this->state += 1;
    return true;
}
int TopologyAccessor::getEdgeID() {
    return this->curr_edge_id;
}
int TopologyAccessor::getOtherID() {
    return this->curr_other_id;
}


void TypedTopologyAccessor::setBaseNode(int node, Direction dir) {
    auto ref = this->topology->node_entries[node];
    if (dir == Direction::FORWARD) {
        this->state = ref.fwd_start;
        this->end = ref.fwd_start + ref.fwd_count;
        this->edge_refs = this->topology->fwd_edge_entries.data();
    }
    else {
        this->state = ref.bwd_start;
        this->end = ref.bwd_start + ref.bwd_count;
        this->edge_refs = this->topology->bwd_edge_entries.data();
    }
}
bool TypedTopologyAccessor::next() {
    if (this->state == this->end) {
        return false;
    }
    auto ref = this->edge_refs[this->state];
    this->curr_edge_id = ref.edge_id;
    this->curr_other_id = ref.other_id;
    this->curr_type = ref.type;
    this->state += 1;
    return true;
}
int TypedTopologyAccessor::getEdgeID() {
    return this->curr_edge_id;
}
int TypedTopologyAccessor::getOtherID() {
    return this->curr_other_id;
}
char TypedTopologyAccessor::getType() {
    return this->curr_type;
}

void DynamicTopologyAccessor::SetBaseNode(int node, Direction dir) {
    auto ref = this->topology->node_entries[node];
    if (dir == Direction::FORWARD) {
        this->state = 0;
        this->end = ref.fwd_edges.size();
        this->edge_refs = ref.fwd_edges.data();
    }
    else {
        this->state = 0;
        this->end = ref.bwd_edges.size();
        this->edge_refs = ref.bwd_edges.data();
    }
}
bool DynamicTopologyAccessor::next() {
    if (this->state == this->end) {
        return false;
    }
    auto ref = this->edge_refs[this->state];
    this->curr_edge_id = ref.edge_id;
    this->curr_other_id = ref.other_id;
    this->curr_type = ref.type;
    this->state += 1;
    return true;
}
int DynamicTopologyAccessor::getEdgeID() {
    return this->curr_edge_id;
}
int DynamicTopologyAccessor::getOtherID() {
    return this->curr_other_id;
}
char DynamicTopologyAccessor::getType() {
    return this->curr_type;
}
