#include <array>
#include <memory>
#include <tuple>
#include <vector>

#include "../structs/enums.h"
#include "./weighting.h"

int Weighting::get_edge_weight(int edge) const
{
    return this->edge_weights[edge];
}
void Weighting::set_edge_weight(int edge, int weight)
{
    this->edge_weights[edge] = weight;
}

std::shared_ptr<Weighting> build_weighting(const GraphBase& base)
{
    std::vector<int> weights(base.edgeCount());
    for (int i = 0; i < base.edgeCount(); i++) {
        weights[i] = 1;
    }

    return std::make_shared<Weighting>(std::move(weights));
}

Vector<float> turn_cost_penalty(const GraphBase& base)
{
    Vector<float> penalties(base.edgeCount());
    for (int i = 0; i < base.edgeCount(); i++) {
        auto edge = base.getEdge(i);
        auto nodeB = edge.nodeB;
        auto degree = base.adjacency.getDegree(nodeB, Direction::FORWARD);
        float w = 0;
        if (degree == 2) {
			w += 2;
		} else if (degree == 3) {
			w += 3;
		} else if (degree == 4) {
			w += 7;
		} else if (degree > 4) {
			w += 10;
		}
        penalties[i] = w;
    }
    return penalties;
}
