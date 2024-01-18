#pragma once

#include <array>
#include <stdexcept>
#include <tuple>

#include "./enums.h"
#include "./geom.h"

struct Edge
{
    int nodeA;
    int nodeB;
    RoadType type;
    float length;
    int maxspeed;
    bool oneway;
};

struct Node
{
    char type;
    Coord location;
};

struct EdgeRef
{
    int edge_id;
    int other_id;
    char type;

    bool isEdge() { return this->type < 100; }
    bool isCrossBorder() { return this->type == 10; }
    bool isSkip() { return this->type == 20; }
    bool isShortcut() { return this->type >= 100; }
    bool isCHShortcut() { return this->type == 100; }
};

template <int size>
struct Data
{
    char data[size];

    Data() : data{0} {}

    template <class T>
    void set(int pos, T value)
    {
#ifndef NDEBUG
        if (pos + sizeof(T) >= size) {
            throw std::invalid_argument("out of bounds");
        }
#endif
        *(static_cast<T*>(static_cast<void*>(&this->data[pos]))) = value;
    }

    template <class T>
    T get(int pos) const
    {
#ifndef NDEBUG
        if (pos + sizeof(T) >= size) {
            throw std::invalid_argument("out of bounds");
        }
#endif
        return *(static_cast<const T*>(static_cast<const void*>(&this->data[pos])));
    }
};

struct Shortcut
{
    int from;
    int to;
    int weight;
    Data<4> payload;
};

// struct CHEdge
// {
//     int from;
//     int to;
//     int weight;
//     short count;
//     bool skip;
// };

// struct CHEdge4
// {
//     int from;
//     int to;
//     int weight;
//     short to_tile;
//     bool is_dummy;
// };

// struct TiledSHEdge
// {
//     int from;
//     int to;
//     int weight;
// };
