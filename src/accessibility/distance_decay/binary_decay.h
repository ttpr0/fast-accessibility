#pragma once

#include "./decay.h"

class BinaryDecay : public IDistanceDecay
{
private:
    int max_distance;
    float factor = 1;

public:
    BinaryDecay(int max_dist) : max_distance(max_dist) {}

    float get_distance_weight(int distance)
    {
        if (distance > this->max_distance) {
            return 0;
        } else {
            return this->factor;
        }
    }
    int get_max_distance() { return this->max_distance; }
};
