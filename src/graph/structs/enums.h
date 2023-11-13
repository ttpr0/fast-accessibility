#pragma once

enum Direction : char
{
    BACKWARD = 0,
    FORWARD = 1,
};

enum RoadType : unsigned char
{
    MOTORWAY = 1,
    MOTORWAY_LINK = 2,
    TRUNK = 3,
    TRUNK_LINK = 4,
    PRIMARY = 5,
    PRIMARY_LINK = 6,
    SECONDARY = 7,
    SECONDARY_LINK = 8,
    TERTIARY = 9,
    TERTIARY_LINK = 10,
    RESIDENTIAL = 11,
    LIVING_STREET = 12,
    UNCLASSIFIED = 13,
    ROAD = 14,
    TRACK = 15,
};

enum Adjacency : char
{
    ADJACENT_EDGES = 0,
    ADJACENT_SHORTCUTS = 1,
    ADJACENT_ALL = 2,
    ADJACENT_SKIP = 3,
    ADJACENT_UPWARDS = 4,
    ADJACENT_DOWNWARDS = 5,
};
