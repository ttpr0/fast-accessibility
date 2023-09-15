#pragma once

#include <math.h>
#include <memory>
#include <optional>
#include <tuple>

//***********************************************************
// declarations
//***********************************************************

class KDNode
{
public:
    float x;
    float y;
    int value;

    std::unique_ptr<KDNode> less;
    std::unique_ptr<KDNode> more;

    KDNode(float x, float y, int value);
};

class KDTree
{
public:
    std::unique_ptr<KDNode> root;

    KDTree();

    std::tuple<int, bool> get_closest(float x, float y, float max_dist);

    void insert(float x, float y, int value);
};
