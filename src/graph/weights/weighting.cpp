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

std::shared_ptr<Weighting> build_default_weighting(const GraphBase& base)
{
    auto weight = build_weighting(base);

    for (int i = 0; i < base.edgeCount(); i++) {
        auto edge = base.edges[i];
        int w = edge.length * 3.6 / edge.maxspeed;
        if (w < 1) {
            w = 1;
        }
        weight->set_edge_weight(i, w);
    }

    return std::move(weight);
}
