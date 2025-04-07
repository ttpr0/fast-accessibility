#pragma once

#include <cmath>
#include <string>
#include <unordered_set>

#include "./driving_profile.h"
#include "./profile.h"

static std::unordered_set<std::string> walking_road_types = {"footway",     "path",          "steps",    "pedestrian",   "living_street", "track",
                                                             "residential", "service",       "primary",  "primary_link", "secondary",     "secondary_link",
                                                             "tertiary",    "tertiary_link", "cycleway", "unclassified", "road"};

class WalkingDecoder : public IOSMDecoder
{
public:
    bool is_valid_highway(const osm::TagMap& tags) const
    {
        if (!tags.contains("highway")) {
            return false;
        }
        const auto& tag = tags.get("highway", "");
        return walking_road_types.contains(tag);
    }
    bool is_oneway(const osm::TagMap& tags) const { return false; }
    NodeAttributes decode_node_tags(const osm::TagMap& tags) const { return NodeAttributes{}; }
    void finalize_node(NodeAttributes& attr, const Coord& geom) const {}
    EdgeAttributes decode_edge_tags(const osm::TagMap& tags) const
    {
        auto str_type = _get_type(tags);
        int speed = 4;
        return EdgeAttributes{.speed = (int8_t)speed, .type = str_type};
    }
    void finalize_edge(EdgeAttributes& attr, const std::vector<Coord>& geom) const { attr.length = _haversine_length(geom); }
};
