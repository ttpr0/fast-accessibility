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
