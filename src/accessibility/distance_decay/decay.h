#pragma once

class IDistanceDecay
{
public:
    virtual ~IDistanceDecay() {}

    virtual float get_distance_weight(int distance) = 0;
    virtual int get_max_distance() = 0;
};
