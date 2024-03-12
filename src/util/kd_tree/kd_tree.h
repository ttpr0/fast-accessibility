#pragma once

#include <math.h>
#include <algorithm>
#include <memory>
#include <optional>
#include <span>
#include <tuple>
#include <vector>

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

struct TreeValue
{
    float coords[2];
    int value;
};

class KDTree
{
public:
    std::unique_ptr<KDNode> root;

    KDTree();

    std::tuple<int, bool> get_closest(float x, float y, float max_dist);

    void insert(float x, float y, int value);

    void create_balanced(std::vector<TreeValue>& values);
};
