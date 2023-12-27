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
