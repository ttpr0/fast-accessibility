#pragma once

#include <cmath>

#include "./decay.h"

class GravityDecay : public IDistanceDecay
{
private:
    int max_distance;
    float beta;

public:
    GravityDecay(int max_dist, float beta) : max_distance(max_dist), beta(beta) {}

    float get_distance_weight(int distance)
    {
        if (distance > this->max_distance) {
            return 0;
        } else {
            return (float)pow(distance, -this->beta);
            ;
        }
    }
    int get_max_distance() { return this->max_distance; }
};
