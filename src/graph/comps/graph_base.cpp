
#include "./graph_base.h"
#include "./graph_index.h"

std::shared_ptr<GraphBase> build_graph_base(std::vector<Node> nodes, std::vector<Edge> edges, std::vector<Coord> node_geoms)
{
    AdjacencyArray adj = build_adjacency_array(nodes, edges);
    KDTree tree = build_balanced_kdtree(node_geoms);

    return std::make_shared<GraphBase>(std::move(nodes), std::move(edges), std::move(node_geoms), std::move(adj), std::move(tree));
}
