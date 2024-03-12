#pragma once

#include <vector>

#include "./decay.h"

class HybridDecay : public IDistanceDecay
{
private:
    std::vector<int> distances;
    std::vector<float> factors;

public:
    HybridDecay(std::vector<int> distances, std::vector<float> factors) : distances(distances), factors(factors) {}

    float get_distance_weight(int distance)
    {
        for (int i = 0; i < this->distances.size(); i++) {
            if (distance <= this->distances[i]) {
                return this->factors[i];
            }
        }
        return 0;
    }
    int get_max_distance() { return this->distances[this->distances.size() - 1]; }
};
