#pragma once

#include <tuple>

#include "./enums.h"

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
};

struct Shortcut
{
    int nodeA;
    int nodeB;
    int edgeref_start;
    int edgeref_count;
};

struct CHShortcut
{
    int nodeA;
    int nodeB;
    std::tuple<int, char> edges[2];
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
