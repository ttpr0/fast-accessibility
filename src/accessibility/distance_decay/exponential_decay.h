#pragma once

#include <cmath>

#include "./decay.h"

class ExponentialDecay : public IDistanceDecay
{
private:
    int max_distance;
    float impedance;

public:
    ExponentialDecay(int max_dist) : max_distance(max_dist) { this->impedance = (float)(-log(0.01) / this->max_distance); }

    float get_distance_weight(int distance)
    {
        if (distance > this->max_distance) {
            return 0;
        } else {
            return (float)exp(-distance * this->impedance);
        }
    }
    int get_max_distance() { return this->max_distance; }
};
