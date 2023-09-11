#pragma once

struct pq_item {
    int node;
    int dist;
};

bool operator<(const pq_item& a, const pq_item& b) {
    return a.dist > b.dist;
}
