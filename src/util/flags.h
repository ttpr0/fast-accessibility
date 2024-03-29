#pragma once

#include <vector>

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
    Flags() {}
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
