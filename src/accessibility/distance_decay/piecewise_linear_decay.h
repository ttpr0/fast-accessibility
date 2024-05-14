#pragma once

#include <vector>

#include "./decay.h"

class PiecewiseLinearDecay : public IDistanceDecay
{
private:
    std::vector<int> distances;
    std::vector<float> factors;

public:
    PiecewiseLinearDecay(std::vector<int> distances, std::vector<float> factors) : distances(distances), factors(factors) {}

    float get_distance_weight(int distance)
    {
        for (int i = 1; i < this->distances.size(); i++) {
            if (distance <= this->distances[i]) {
                float m = (this->factors[i] - this->factors[i-1]) / (this->distances[i] - this->distances[i-1]);
                return m * (distance - this->distances[i-1]) + this->factors[i-1];
            }
        }
        return 0;
    }
    int get_max_distance() { return this->distances[this->distances.size() - 1]; }
};
