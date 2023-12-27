#pragma once

#include <array>
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

struct Shortcut
{
    int from;
    int to;
    int weight;
    std::array<char, 4> payload;

    template <class T>
    void set(T value, int pos)
    {
        *(static_cast<T*>(this->payload[pos])) = value;
    }

    template <class T>
    T get(int pos)
    {
        return *(static_cast<T*>(this->payload[pos]));
    }
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

struct CHEdge
{
    int from;
    int to;
    int weight;
    short count;
    bool skip;
};

struct CHEdge4
{
    int from;
    int to;
    int weight;
    short to_tile;
    bool is_dummy;
};

struct TiledSHEdge
{
    int from;
    int to;
    int weight;
};
