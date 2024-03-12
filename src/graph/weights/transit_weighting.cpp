#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "../structs/enums.h"
#include "./transit_weighting.h"

std::optional<ConnectionWeight> TransitWeighting::get_next_weight(int connection, int from) const
{
    auto& conn_weights = this->transit_weights[connection];
    for (int i = 0; i < conn_weights.size(); i++) {
        if (conn_weights[i].departure >= from) {
            return conn_weights[i];
        }
    }
    return std::nullopt;
}
std::span<const ConnectionWeight> TransitWeighting::get_weights_in_range(int connection, int from, int to) const
{
    auto& conn_weights = this->transit_weights[connection];
    int start = -1;
    int end = -1;
    for (int i = 0; i < conn_weights.size(); i++) {
        if (conn_weights[i].departure >= from && start == -1) {
            start = i;
        }
        if (start != -1 && conn_weights[i].departure > to) {
            end = i;
            break;
        }
    }
    if (start == -1) {
        return {};
    }
    if (end == -1) {
        end = conn_weights.size();
    }
    return std::span<const ConnectionWeight>(conn_weights.data() + start, end - start);
}
void TransitWeighting::set_weights(int connection, std::vector<ConnectionWeight> weights)
{
    std::sort(weights.begin(), weights.end(), [](const ConnectionWeight& w1, const ConnectionWeight& w2) { return w1.departure < w2.departure; });
    this->transit_weights[connection] = std::move(weights);
}

std::shared_ptr<TransitWeighting> build_transit_weighting(const TransitData& base)
{
    std::vector<std::vector<ConnectionWeight>> weights(base.connectionCount());
    return std::make_shared<TransitWeighting>(std::move(weights));
}
