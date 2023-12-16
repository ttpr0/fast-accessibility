
#include "./graph_index.h"

//*******************************************
// graph index
//*******************************************

std::tuple<int, bool> BaseGraphIndex::getClosestNode(Coord point)
{
    int closest = -1;
    int min_dist = 100000;
    for (int i = 0; i < this->node_geoms.size(); i++) {
        Coord other = this->node_geoms[i];
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

KDTree build_kdtree(std::vector<Coord>& node_geoms)
{
    auto tree = KDTree();
    for (int i = 0; i < node_geoms.size(); i++) {
        auto coord = node_geoms[i];
        tree.insert(coord.lon, coord.lat, i);
    }
    return tree;
}

KDTree build_balanced_kdtree(std::vector<Coord>& node_geoms)
{
    auto tree = KDTree();
    std::vector<TreeValue> values(node_geoms.size());
    for (int i = 0; i < node_geoms.size(); i++) {
        auto coord = node_geoms[i];
        values[i] = {{coord.lon, coord.lat}, i};
    }
    tree.create_balanced(values);
    return tree;
}

std::shared_ptr<IGraphIndex> build_base_index(GraphBase& base)
{
    return std::make_shared<BaseGraphIndex>(base.node_geoms);
}
std::shared_ptr<IGraphIndex> build_kdtree_index(GraphBase& base)
{
    return std::make_shared<KDTreeIndex>(build_kdtree(base.node_geoms));
}

std::shared_ptr<IGraphIndex> build_balanced_kdtree_index(GraphBase& base)
{
    return std::make_shared<KDTreeIndex>(build_balanced_kdtree(base.node_geoms));
}
