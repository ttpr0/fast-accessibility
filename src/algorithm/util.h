#pragma once

#include <vector>

struct pq_item
{
    int node;
    int dist;
};

static bool operator<(const pq_item& a, const pq_item& b)
{
    return a.dist > b.dist;
}

struct DistFlag
{
    int dist;
    bool visited;
};

struct TransitFlag
{
    std::vector<std::tuple<int, int>> trips;
};

template <typename T>
class Flags
{
private:
    struct Flag
    {
        T data;
        unsigned short _counter;
    };

    std::vector<Flag> flags;
    T _default;
    unsigned short _counter;

public:
    Flags(int size, T _default) : flags(size), _default(_default), _counter(0) { this->hard_reset(); }

    inline T& operator[](int pos) noexcept
    {
        Flag& flag = this->flags[pos];
        if (flag._counter != this->_counter) {
            flag = {this->_default, this->_counter};
        }
        return flag.data;
    }

    void soft_reset()
    {
        this->_counter += 1;
        if (this->_counter > 10000) {
            this->hard_reset();
        }
    }

    void hard_reset()
    {
        int size = this->flags.size();
        for (int i = 0; i < size; i++) {
            this->flags[i] = {this->_default, 0};
        }
        this->_counter = 0;
    }
};

struct Snap
{
    int node;
    int dist;
};

struct DSnap
{
    Snap snaps[2];

    DSnap()
    {
        this->snaps[0] = {-1, 0};
        this->snaps[1] = {-1, 0};
    }
    DSnap(int node)
    {
        this->snaps[0] = {node, 0};
        this->snaps[1] = {-1, 0};
    }
    DSnap(Snap snap)
    {
        this->snaps[0] = snap;
        this->snaps[1] = {-1, 0};
    }
    DSnap(Snap snap1, Snap snap2)
    {
        this->snaps[0] = snap1;
        this->snaps[1] = snap2;
    }

    int len()
    {
        if (snaps[0].node == -1) {
            return 0;
        }
        if (snaps[1].node == -1) {
            return 1;
        }
        return 2;
    }

    Snap operator[](int i) { return this->snaps[i]; }
};
