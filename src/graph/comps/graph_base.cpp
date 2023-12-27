
#include "./graph_base.h"
#include "./graph_index.h"

std::shared_ptr<GraphBase> new_graph_base(std::vector<Node> nodes, std::vector<Edge> edges)
{
    AdjacencyArray adj = build_adjacency_array(nodes, edges);

    return std::make_shared<GraphBase>(std::move(nodes), std::move(edges), std::move(adj));
}
