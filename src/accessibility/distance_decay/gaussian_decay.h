#pragma once

#include <cmath>

#include "./decay.h"

class GaussianDecay : public IDistanceDecay
{
private:
    int max_distance;
    float impedance;

public:
    GaussianDecay(int max_dist) : max_distance(max_dist) { this->impedance = (float)(-pow(max_distance, 2) / log(0.01)); }

    float get_distance_weight(int distance)
    {
        if (distance > this->max_distance) {
            return 0;
        } else {
            return (float)exp(-pow(distance, 2) / this->impedance);
        }
    }
    int get_max_distance() { return this->max_distance; }
};
