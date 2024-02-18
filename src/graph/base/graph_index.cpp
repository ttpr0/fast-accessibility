
#include "./graph_index.h"

//*******************************************
// graph index
//*******************************************

std::tuple<int, bool> BaseGraphIndex::getClosestNode(Coord point)
{
    int closest = -1;
    int min_dist = 100000;
    for (int i = 0; i < this->nodes.size(); i++) {
        Coord other = this->nodes[i].location;
        float d_lon = other.lon - point.lon;
        float d_lat = other.lat - point.lat;
        float dist = sqrt(d_lon * d_lon + d_lat * d_lat);
        if (dist < min_dist) {
            min_dist = dist;
            closest = i;
        }
    }
    return std::make_tuple(closest, true);
}

std::tuple<int, bool> KDTreeIndex::getClosestNode(Coord point)
{
    return this->tree.get_closest(point.lon, point.lat, 0.01);
}

KDTree build_kdtree(std::vector<Node>& nodes)
{
    auto tree = KDTree();
    for (int i = 0; i < nodes.size(); i++) {
        auto coord = nodes[i].location;
        tree.insert(coord.lon, coord.lat, i);
    }
    return tree;
}

KDTree build_balanced_kdtree(std::vector<Node>& nodes)
{
    auto tree = KDTree();
    std::vector<TreeValue> values(nodes.size());
    for (int i = 0; i < nodes.size(); i++) {
        auto coord = nodes[i].location;
        values[i] = {{coord.lon, coord.lat}, i};
    }
    tree.create_balanced(values);
    return tree;
}

std::shared_ptr<IGraphIndex> build_base_index(GraphBase& base)
{
    return std::make_shared<BaseGraphIndex>(base.nodes);
}
std::shared_ptr<IGraphIndex> build_kdtree_index(GraphBase& base)
{
    return std::make_shared<KDTreeIndex>(build_kdtree(base.nodes));
}

std::shared_ptr<IGraphIndex> build_balanced_kdtree_index(GraphBase& base)
{
    return std::make_shared<KDTreeIndex>(build_balanced_kdtree(base.nodes));
}