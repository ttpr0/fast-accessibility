#pragma once

#include "./decay.h"

class LinearDecay : public IDistanceDecay
{
private:
    int max_distance;

public:
    LinearDecay(int max_dist) : max_distance(max_dist) {}

    float get_distance_weight(int distance)
    {
        if (distance >= this->max_distance) {
            return 0;
        } else {
            return 1 - ((float)distance / this->max_distance);
        }
    }
    int get_max_distance() { return this->max_distance; }
};
