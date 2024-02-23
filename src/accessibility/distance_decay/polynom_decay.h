#pragma once

#include <vector>

#include "./decay.h"

class PolynomDecay : public IDistanceDecay
{
private:
    int max_distance;
    std::vector<float> koefficients;

public:
    PolynomDecay(int max_distance, std::vector<float> koefficients) : max_distance(max_distance), koefficients(koefficients) {}

    float get_distance_weight(int distance)
    {
        if (distance >= this->max_distance) {
            return 0;
        }
        int degree = this->koefficients.size();
        double weight = this->koefficients[0];
        for (int i = 1; i < degree; i++) {
            weight += pow(distance, i) * this->koefficients[i];
        }
        return (float)weight;
    }
    int get_max_distance() { return this->max_distance; }
};
