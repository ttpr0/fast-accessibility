#pragma once

#include <cmath>
#include <string>
#include <unordered_set>

#include "./profile.h"

static std::unordered_set<std::string> road_types = {"motorway",  "motorway_link",  "trunk",        "trunk_link",    "primary",     "primary_link",
                                                     "secondary", "secondary_link", "tertiary",     "tertiary_link", "residential", "living_street",
                                                     "service",   "track",          "unclassified", "road",          "motorroad"};

bool _is_oneway(const osm::TagMap& tags, StreetType str_type)
{
    if (str_type == StreetType::MOTORWAY || str_type == StreetType::TRUNK || str_type == StreetType::MOTORWAY_LINK || str_type == StreetType::TRUNK_LINK) {
        return true;
    }
    return tags.get("oneway", "") == "yes";
}

StreetType _get_type(const osm::TagMap& tags)
{
    std::string type = tags.get("highway", "");
    if (type == "motorway") {
        return StreetType::MOTORWAY;
    } else if (type == "trunk") {
        return StreetType::TRUNK;
    } else if (type == "motorway_link") {
        return StreetType::MOTORWAY_LINK;
    } else if (type == "trunk_link") {
        return StreetType::TRUNK_LINK;
    } else if (type == "primary") {
        return StreetType::PRIMARY;
    } else if (type == "primary_link") {
        return StreetType::PRIMARY_LINK;
    } else if (type == "secondary") {
        return StreetType::SECONDARY;
    } else if (type == "secondary_link") {
        return StreetType::SECONDARY_LINK;
    } else if (type == "tertiary") {
        return StreetType::TERTIARY;
    } else if (type == "tertiary_link") {
        return StreetType::TERTIARY_LINK;
    } else if (type == "residential") {
        return StreetType::RESIDENTIAL;
    } else if (type == "living_street") {
        return StreetType::LIVING_STREET;
    } else if (type == "service") {
        return StreetType::SERVICE;
    } else if (type == "track") {
        return StreetType::TRACK;
    } else if (type == "unclassified") {
        return StreetType::UNCLASSIFIED;
    } else if (type == "road") {
        return StreetType::ROAD;
    } else {
        return StreetType::UNCLASSIFIED;
    }
}

int _get_speed(const osm::TagMap& tags, StreetType str_type)
{
    auto tracktype = tags.get("tracktype", "");
    auto surface = tags.get("surface", "");
    int speed = 0;
    // check if maxspeed is set
    if (tags.contains("maxspeed") || tags.contains("maxspeed:forward") || tags.contains("maxspeed:backward")) {
        std::string templimit = "";
        if (tags.contains("maxspeed")) {
            templimit = tags.get("maxspeed", "");
        } else if (tags.contains("maxspeed:forward")) {
            templimit = tags.get("maxspeed:forward", "");
        } else {
            templimit = tags.get("maxspeed:backward", "");
        }
        if (templimit == "walk") {
            speed = 10;
        } else if (templimit == "none") {
            speed = 110;
        } else {
            try {
                speed = std::stoi(templimit);
            } catch (...) {
                speed = 20;
            }
            speed = int(0.9 * speed);
        }
    } else if (tags.contains("zone:maxspeed")) {
        try {
            auto templimit_zone = tags.get("zone:maxspeed", "");
            auto templimit = templimit_zone.substr(templimit_zone.find(":") + 1);
            speed = std::stoi(templimit);
        } catch (...) {
            speed = 20;
        }
        speed = int(0.9 * speed);
    } else if (tags.contains("zone:traffic")) {
        try {
            auto traffic_zone = tags.get("zone:traffic", "");
            auto zone = traffic_zone.substr(traffic_zone.find(":") + 1);
            if (zone == "motorway") {
                speed = 130;
            } else if (zone == "rural") {
                speed = 100;
            } else if (zone == "urban") {
                speed = 50;
            } else if (zone == "living_street") {
                speed = 10;
            } else {
                speed = 20;
            }
        } catch (...) {
            speed = 20;
        }
        speed = int(0.9 * speed);
    } else {
        // is templimit not set take maxspeed from streetype
        switch (str_type) {
            case StreetType::MOTORWAY:
                speed = 100;
                break;
            case StreetType::TRUNK:
                speed = 85;
                break;
            case StreetType::MOTORWAY_LINK:
                speed = 60;
                break;
            case StreetType::TRUNK_LINK:
                speed = 60;
                break;
            case StreetType::PRIMARY:
                speed = 65;
                break;
            case StreetType::SECONDARY:
                speed = 60;
                break;
            case StreetType::TERTIARY:
                speed = 50;
                break;
            case StreetType::PRIMARY_LINK:
                speed = 50;
                break;
            case StreetType::SECONDARY_LINK:
                speed = 50;
                break;
            case StreetType::TERTIARY_LINK:
                speed = 40;
                break;
            case StreetType::UNCLASSIFIED:
                speed = 30;
                break;
            case StreetType::RESIDENTIAL:
                speed = 30;
                break;
            case StreetType::LIVING_STREET:
                speed = 10;
                break;
            case StreetType::ROAD:
                speed = 20;
                break;
            case StreetType::TRACK:
                if (tracktype == "") {
                    speed = 15;
                } else {
                    if (tracktype == "grade1") {
                        speed = 40;
                    } else if (tracktype == "grade2") {
                        speed = 30;
                    } else if (tracktype == "grade3") {
                        speed = 20;
                    } else if (tracktype == "grade4") {
                        speed = 15;
                    } else {
                        speed = 15;
                    }
                }
                break;
            default:
                speed = 20;
                break;
        }
        // reduce speed to maximum of given surface
        if (surface == "cement" || surface == "compacted") {
            if (speed > 80) {
                speed = 80;
            }
        } else if (surface == "fine_gravel") {
            if (speed > 60) {
                speed = 60;
            }
        } else if (surface == "paving_stones" || surface == "metal" || surface == "bricks") {
            if (speed > 40) {
                speed = 40;
            }
        } else if (surface == "grass" || surface == "wood" || surface == "sett" || surface == "grass_paver" || surface == "gravel" || surface == "unpaved" || surface == "ground" ||
                   surface == "dirt" || surface == "pebblestone" || surface == "tartan") {
            if (speed > 30) {
                speed = 30;
            }
        } else if (surface == "cobblestone" || surface == "clay") {
            if (speed > 20) {
                speed = 20;
            }
        } else if (surface == "earth" || surface == "stone" || surface == "rocky" || surface == "sand") {
            if (speed > 15) {
                speed = 15;
            }
        } else if (surface == "mud") {
            if (speed > 10) {
                speed = 10;
            }
        }
    }
    if (speed <= 0) {
        speed = 10;
    }
    if (speed > 125) {
        speed = 125;
    }
    return speed;
}

#define R 6365000
#define PI 3.14159265358979323846

float _haversine_length(const std::vector<Coord>& points)
{
    float length = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        float lat1 = points[i].lat * PI / 180;
        float lat2 = points[i + 1].lat * PI / 180;
        float lon1 = points[i].lon * PI / 180;
        float lon2 = points[i + 1].lon * PI / 180;
        float a = pow(sin((lat2 - lat1) / 2), 2);
        float b = pow(sin((lon2 - lon1) / 2), 2);
        length += 2 * R * asin(sqrt(a + cos(lat1) * cos(lat2) * b));
    }
    return length;
}

class DrivingDecoder : public IOSMDecoder
{
public:
    bool is_valid_highway(const osm::TagMap& tags) const
    {
        if (!tags.contains("highway")) {
            return false;
        }
        const auto& tag = tags.get("highway", "");
        return road_types.contains(tag);
    }
    bool is_oneway(const osm::TagMap& tags) const
    {
        auto str_type = _get_type(tags);
        return _is_oneway(tags, str_type);
    }
    NodeAttributes decode_node_tags(const osm::TagMap& tags) const { return NodeAttributes{}; }
    void finalize_node(NodeAttributes& attr, const Coord& geom) const {}
    EdgeAttributes decode_edge_tags(const osm::TagMap& tags) const
    {
        auto str_type = _get_type(tags);
        int speed = _get_speed(tags, str_type);
        return EdgeAttributes{.speed = (int8_t)speed, .type = str_type};
    }
    void finalize_edge(EdgeAttributes& attr, const std::vector<Coord>& geom) const { attr.length = _haversine_length(geom); }
};
