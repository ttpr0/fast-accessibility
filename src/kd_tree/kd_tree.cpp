#include "kd_tree.h"

void insert_node(KDNode& node, std::unique_ptr<KDNode> new_node, int dim)
{
    float val;
    float new_val;
    int new_dim;

    switch (dim) {
        case 0:
            val = node.x;
            new_val = new_node->x;
            new_dim = 1;
            break;
        case 1:
            val = node.y;
            new_val = new_node->y;
            new_dim = 0;
            break;
        default:
            return;
    }

    if (val >= new_val) {
        if (!(node.less)) {
            node.less = std::move(new_node);
        } else {
            insert_node(*node.less, std::move(new_node), new_dim);
        }
    } else {
        if (!(node.more)) {
            node.more = std::move(new_node);
        } else {
            insert_node(*node.more, std::move(new_node), new_dim);
        }
    }
}

std::unique_ptr<KDNode> insert_nodes_balanced(std::span<TreeValue> values, int dim)
{
    int len = values.size();
    if (len < 1) {
        return nullptr;
    }
    if (len == 1) {
        auto value = values[0];
        return std::make_unique<KDNode>(value.coords[0], value.coords[1], value.value);
    }

    std::sort(values.begin(), values.end(), [&dim](const TreeValue& v1, const TreeValue& v2) { return v1.coords[dim] < v2.coords[dim]; });

    int mid = len / 2;
    auto value = values[mid];
    auto new_node = std::make_unique<KDNode>(value.coords[0], value.coords[1], value.value);

    int new_dim;
    switch (dim) {
        case 0:
            new_dim = 1;
            break;
        case 1:
            new_dim = 0;
            break;
        default:
            return nullptr;
    }
    auto left_half = std::span(values.data(), mid);
    new_node->less = insert_nodes_balanced(left_half, new_dim);
    auto right_half = std::span(&values.data()[mid + 1], len - mid);
    new_node->more = insert_nodes_balanced(right_half, new_dim);

    return new_node;
}

std::tuple<std::optional<KDNode*>, double> get_closest_node(KDNode& node, int dim, float x, float y, float max_dist)
{
    KDNode* closest = &node;

    auto dist = sqrt((node.x - x) * (node.x - x) + (node.y - y) * (node.y - y));

    float val;
    float s_val;
    int new_dim;

    switch (dim) {
        case 0:
            val = node.x;
            s_val = x;
            new_dim = 1;
            break;
        case 1:
            val = node.y;
            s_val = y;
            new_dim = 0;
            break;
        default:
            return std::make_tuple(std::nullopt, -1);
    }

    if ((val - s_val) < max_dist) {
        if (!node.more) {
            return std::make_tuple(std::nullopt, -1);
        }
        auto [new_closest, new_dist] = get_closest_node(*node.more, new_dim, x, y, max_dist);
        if (new_closest.has_value() && new_dist < dist) {
            closest = new_closest.value();
            dist = new_dist;
        }
    }
    if ((s_val - val) < max_dist) {
        if (!node.less) {
            return std::make_tuple(std::nullopt, -1);
        }
        auto [new_closest, new_dist] = get_closest_node(*node.less, new_dim, x, y, max_dist);
        if (new_closest && new_dist < dist) {
            closest = new_closest.value();
            dist = new_dist;
        }
    }

    if (dist > max_dist) {
        return std::make_tuple(std::nullopt, -1);
    } else {
        return std::make_tuple(closest, dist);
    }
}

KDNode::KDNode(float x, float y, int value)
{
    this->x = x;
    this->y = y;
    this->value = value;
    this->less = nullptr;
    this->more = nullptr;
}

KDTree::KDTree()
{
    this->root = nullptr;
}

std::tuple<int, bool> KDTree::get_closest(float x, float y, float max_dist)
{
    auto [node, _] = get_closest_node(*this->root, 0, x, y, max_dist);

    if (!node) {
        return std::make_tuple(-1, false);
    } else {
        return std::make_tuple(node.value()->value, true);
    }
}

void KDTree::insert(float x, float y, int value)
{
    if (!this->root) {
        auto new_node = std::make_unique<KDNode>(x, y, value);
        this->root = std::move(new_node);
    } else {
        auto new_node = std::make_unique<KDNode>(x, y, value);
        insert_node(*this->root, std::move(new_node), 0);
    }
}

void KDTree::create_balanced(std::vector<TreeValue>& values)
{
    this->root = insert_nodes_balanced(values, 0);
}
