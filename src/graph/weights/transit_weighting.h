#pragma once

#include <array>
#include <memory>
#include <optional>
#include <span>
#include <tuple>
#include <vector>

#include "../speed_ups/transit_data.h"

//*******************************************
// weighting class with turn-costs
//*******************************************

struct ConnectionWeight
{
    int departure;
    int arrival;
    int trip;
};

class TransitWeighting
{
public:
    std::vector<std::vector<ConnectionWeight>> transit_weights;

    std::optional<ConnectionWeight> get_next_weight(int connection, int from) const;
    std::span<const ConnectionWeight> get_weights_in_range(int connection, int from, int to) const;
    void set_weights(int connection, std::vector<ConnectionWeight> weights);
};

std::shared_ptr<TransitWeighting> build_transit_weighting(const TransitData& base);
