#pragma once

#include <cmath>

#include "./decay.h"

class KernelDensityDecay : public IDistanceDecay
{
private:
    int max_distance;
    float factor;

public:
    KernelDensityDecay(int max_dist, float factor) : max_distance(max_dist), factor(factor) {}

    float get_distance_weight(int distance)
    {
        if (distance > this->max_distance) {
            return 0;
        } else {
            return this->factor * (1 - (float)pow(distance / this->max_distance, 2));
        }
    }
    int get_max_distance() { return this->max_distance; }
};
