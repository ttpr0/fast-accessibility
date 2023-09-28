#pragma once

#include <vector>

struct pq_item
{
    int node;
    int dist;
};

bool operator<(const pq_item& a, const pq_item& b)
{
    return a.dist > b.dist;
}

struct DistFlag
{
    int dist;
    bool visited;
    short _flag_counter;
};

class DistFlagArray
{
private:
    std::vector<DistFlag> flags;
    short _counter;

public:
    DistFlagArray(int size)
    {
        this->flags = std::vector<DistFlag>(size);
        this->_counter = 0;
        this->hard_reset();
    }

    void set_start(int start) { this->flags[start] = {0, false, this->_counter}; }

    inline constexpr DistFlag& operator[](int pos) noexcept
    {
        DistFlag& flag = this->flags[pos];
        if (flag._flag_counter != this->_counter) {
            flag = {1000000000, false, this->_counter};
        }
        return flag;
    }

    std::vector<DistFlag>& get_flags() { return this->flags; }
    short get_counter() { return this->_counter; }

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
            this->flags[i] = {1000000000, false, 0};
        }
        this->_counter = 0;
    }
};