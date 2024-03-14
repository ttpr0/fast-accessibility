
#include "./graph_index.h"

//*******************************************
// graph index
//*******************************************

DSnap KDTreeIndex::getClosestNode(Coord point)
{
    auto [node, ok] = this->tree.get_closest(point.lon, point.lat, 0.01);
    if (!ok) {
        return {};
    }
    return {node};
}
DSnap KDTreeIndex::getClosestNode(float lon, float lat)
{
    auto [node, ok] = this->tree.get_closest(lon, lat, 0.01);
    if (!ok) {
        return {};
    }
    return {node};
}
DSnap KDTreeIndex::getClosestNode(Coord point, const _IDMapping& id_mapping)
{
    auto [node, ok] = this->tree.get_closest(point.lon, point.lat, 0.01);
    if (!ok) {
        return {};
    }
    auto m_node = id_mapping.get_target(node);
    return {m_node};
}
DSnap KDTreeIndex::getClosestNode(float lon, float lat, const _IDMapping& id_mapping)
{
    auto [node, ok] = this->tree.get_closest(lon, lat, 0.01);
    if (!ok) {
        return {};
    }
    auto m_node = id_mapping.get_target(node);
    return {m_node};
}

std::vector<DSnap> KDTreeIndex::mapToClosest(const std::vector<Coord>& coords)
{
    std::vector<DSnap> closest(coords.size());
    for (int i = 0; i < coords.size(); i++) {
        auto node = this->getClosestNode(coords[i]);
        closest[i] = {node};
    }
    return closest;
}
std::vector<DSnap> KDTreeIndex::mapToClosest(VectorView<float> lons, VectorView<float> lats)
{
    std::vector<DSnap> closest(lons.rows());
    for (int i = 0; i < lons.rows(); i++) {
        auto node = this->getClosestNode({lons(i), lats(i)});
        closest[i] = {node};
    }
    return closest;
}
std::vector<DSnap> KDTreeIndex::mapToClosest(const std::vector<Coord>& coords, const _IDMapping& id_mapping)
{
    std::vector<DSnap> closest(coords.size());
    for (int i = 0; i < coords.size(); i++) {
        auto node = this->getClosestNode(coords[i], id_mapping);
        closest[i] = {node};
    }
    return closest;
}
std::vector<DSnap> KDTreeIndex::mapToClosest(VectorView<float> lons, VectorView<float> lats, const _IDMapping& id_mapping)
{
    std::vector<DSnap> closest(lons.rows());
    for (int i = 0; i < lons.rows(); i++) {
        auto node = this->getClosestNode({lons(i), lats(i)}, id_mapping);
        closest[i] = {node};
    }
    return closest;
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

std::shared_ptr<IGraphIndex> build_kdtree_index(GraphBase& base)
{
    return std::make_shared<KDTreeIndex>(build_kdtree(base.nodes));
}

std::shared_ptr<IGraphIndex> build_balanced_kdtree_index(GraphBase& base)
{
    return std::make_shared<KDTreeIndex>(build_balanced_kdtree(base.nodes));
}
