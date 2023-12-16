
#include "./graph_base.h"
#include "./graph_index.h"

std::shared_ptr<GraphBase> new_graph_base(std::vector<Node> nodes, std::vector<Edge> edges, std::vector<Coord> node_geoms)
{
    AdjacencyArray adj = build_adjacency_array(nodes, edges);

    return std::make_shared<GraphBase>(std::move(nodes), std::move(edges), std::move(node_geoms), std::move(adj));
}
