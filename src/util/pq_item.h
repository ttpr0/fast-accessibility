#pragma once

template <typename T, typename P>
struct pq
{
    T val;
    P prio;
};

template <typename T, typename P>
bool operator<(const pq<T, P>& a, const pq<T, P>& b)
{
    return a.prio > b.prio;
}

struct pq_item
{
    int node;
    int dist;
};

static bool operator<(const pq_item& a, const pq_item& b)
{
    return a.dist > b.dist;
}
