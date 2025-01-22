#pragma once

#include <unordered_map>
#include <vector>

#include "../../graph/structs/geom.h"
#include "./parser.h"

struct NodeAttributes
{
    int8_t type;
};

enum class StreetType {
    MOTORWAY = 0,
    MOTORWAY_LINK = 1,
    TRUNK = 2,
    TRUNK_LINK = 3,
    PRIMARY = 4,
    PRIMARY_LINK = 5,
    SECONDARY = 6,
    SECONDARY_LINK = 7,
    TERTIARY = 8,
    TERTIARY_LINK = 9,
    RESIDENTIAL = 10,
    LIVING_STREET = 11,
    UNCLASSIFIED = 12,
    ROAD = 13,
    TRACK = 14,
    SERVICE = 15
};

struct EdgeAttributes
{
    int8_t speed;
    float length;
    StreetType type;
};

class IOSMDecoder
{
public:
    virtual bool is_valid_highway(const osm::TagMap& tags) const = 0;
    virtual bool is_oneway(const osm::TagMap& tags) const = 0;
    virtual NodeAttributes decode_node_tags(const osm::TagMap& tags) const = 0;
    virtual void finalize_node(NodeAttributes& attr, const Coord& geom) const = 0;
    virtual EdgeAttributes decode_edge_tags(const osm::TagMap& tags) const = 0;
    virtual void finalize_edge(EdgeAttributes& attr, const std::vector<Coord>& geom) const = 0;
};
