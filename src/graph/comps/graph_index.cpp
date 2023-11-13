
#include "./graph_index.h"

//*******************************************
// graph index
//*******************************************

int BaseGraphIndex::getClosestNode(Coord point)
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
    return closest;
}

int KDTreeIndex::getClosestNode(Coord point)
{
    auto [id, ok] = this->tree.get_closest(point.lon, point.lat, 0.01);
    if (ok) {
        return id;
    }
    return -1;
}

int MappedKDTreeIndex::getClosestNode(Coord point)
{
    auto [id, ok] = this->tree.get_closest(point.lon, point.lat, 0.01);
    if (ok) {
        int m_id = this->id_mapping.get_target(id);
        return m_id;
    }
    return -1;
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

// std::unique_ptr<IGraphIndex> build_kdtree_index(std::vector<Coord>& node_geoms)
// {
//     return std::make_unique<KDTreeIndex>(node_geoms);
// }

// std::unique_ptr<IGraphIndex> build_mapped_kdtree_index(std::vector<Coord>& node_geoms, _IDMapping id_mapping)
// {
//     return std::make_unique<MappedKDTreeIndex>(node_geoms, std::move(id_mapping));
// }
