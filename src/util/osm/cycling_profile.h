#pragma once

#include <cmath>
#include <string>
#include <unordered_set>

#include "./driving_profile.h"
#include "./profile.h"

static std::unordered_set<std::string> cycling_road_types = {"trunk",         "trunk_link",   "primary",       "primary_link", "secondary", "secondary_link", "tertiary",
                                                             "tertiary_link", "unclassified", "living_street", "steps",        "path",      "residential",    "road",
                                                             "service",       "footway",      "pedestrian",    "track",        "cycleway"};

/**
def _get_speed(templimit: str, streettype: str, tracktype: str, surface: str) -> int:
    speed = 0
    # is templimit not set take maxspeed from streettype
    if templimit == "":
        match streettype:
            case "trunk" | "trunk_link" | "primary" | "primary_link" | "secondary" | "secondary_link" | "tertiary" | "tertiary_link" | "cycleway" | "residential":
                speed = 18
            case "unclassified":
                speed = 16
            case "service":
                speed = 14
            case "living_street" | "road":
                speed = 12
            case "path":
                speed = 10
            case "footway" | "pedestrian":
                speed = 6
            case "steps":
                speed = 2
            case "track":
                if tracktype == "":
                    speed = 15
                else:
                    match tracktype:
                        case "grade1":
                            speed = 18
                        case "grade2":
                            speed = 12
                        case "grade3":
                            speed = 8
                        case "grade4":
                            speed = 6
                        case "grade5":
                            speed = 4
                        case _:
                            speed = 12
            case _:
                speed = 12
    # reduce speed to maximum of given surface
    match surface:
        case "asphalt" | "concrete" | "fine_gravel" | "paved":
            speed = min(speed, 18)
        case "concrete:plates" | "concrete:lanes" | "compacted" | "pebblestone":
            speed = min(speed, 16)
        case "unpaved":
            speed = min(speed, 18)
        case "earth" | "gravel" | "ground" | "paving_stones" | "paving_stones:30":
            speed = min(speed, 12)
        case "cobblestone:flattened" | "dirt" | "metal" | "mud" | "sett":
            speed = min(speed, 10)
        case "cobblestone" | "grass" | "grass_paver":
            speed = min(speed, 8)
        case "cement" | "clay" | "sand" | "salt" | "unknown" | "wood":
            speed = min(speed, 6)
        case "ice":
            speed = min(speed, 2)
    # if speed is 0 set it to minimum value
    if speed == 0:
        speed = 6
    return speed
*/

int _get_speed(const std::string& templimit, const std::string& streettype, const std::string& tracktype, const std::string& surface)
{
    int speed = 0;
    // is templimit not set take maxspeed from streettype
    if (templimit == "") {
        if (streettype == "trunk" || streettype == "trunk_link" || streettype == "primary" || streettype == "primary_link" || streettype == "secondary" ||
            streettype == "secondary_link" || streettype == "tertiary" || streettype == "tertiary_link" || streettype == "cycleway" || streettype == "residential") {
            speed = 18;
        } else if (streettype == "unclassified") {
            speed = 16;
        } else if (streettype == "service") {
            speed = 14;
        } else if (streettype == "living_street" || streettype == "road") {
            speed = 12;
        } else if (streettype == "path") {
            speed = 10;
        } else if (streettype == "footway" || streettype == "pedestrian") {
            speed = 6;
        } else if (streettype == "steps") {
            speed = 2;
        } else if (streettype == "track") {
            if (tracktype == "") {
                speed = 15;
            } else {
                if (tracktype == "grade1") {
                    speed = 18;
                } else if (tracktype == "grade2") {
                    speed = 12;
                } else if (tracktype == "grade3") {
                    speed = 8;
                } else if (tracktype == "grade4") {
                    speed = 6;
                } else if (tracktype == "grade5") {
                    speed = 4;
                } else {
                    speed = 12;
                }
            }
        }
    }
    // reduce speed to maximum of given surface
    if (surface == "asphalt" || surface == "concrete" || surface == "fine_gravel" || surface == "paved") {
        speed = std::min(speed, 18);
    } else if (surface == "concrete:plates" || surface == "concrete:lanes" || surface == "compacted" || surface == "pebblestone") {
        speed = std::min(speed, 16);
    } else if (surface == "unpaved") {
        speed = std::min(speed, 18);
    } else if (surface == "earth" || surface == "gravel" || surface == "ground" || surface == "paving_stones" || surface == "paving_stones:30") {
        speed = std::min(speed, 12);
    } else if (surface == "cobblestone:flattened" || surface == "dirt" || surface == "metal" || surface == "mud" || surface == "sett") {
        speed = std::min(speed, 10);
    } else if (surface == "cobblestone" || surface == "grass" || surface == "grass_paver") {
        speed = std::min(speed, 8);
    } else if (surface == "cement" || surface == "clay" || surface == "sand" || surface == "salt" || surface == "unknown" || surface == "wood") {
        speed = std::min(speed, 6);
    } else if (surface == "ice") {
        speed = std::min(speed, 2);
    }
    // if speed is 0 set it to minimum value
    if (speed == 0) {
        speed = 6;
    }
    return speed;
}

class CyclingDecoder : public IOSMDecoder
{
public:
    bool is_valid_highway(const osm::TagMap& tags) const
    {
        if (!tags.contains("highway")) {
            return false;
        }
        if (tags.get("motorway", "") == "yes") {
            return false;
        }
        if (tags.get("highway", "") == "cycleway" || tags.get("bicycle_road", "") == "yes") {
            return true;
        }
        const auto& bicyle_tag = tags.get("bicycle", "");
        if (bicyle_tag == "yes" || bicyle_tag == "designated" || bicyle_tag == "official" || bicyle_tag == "permissive") {
            return true;
        }
        for (const auto& key : {"bicycle", "vehicle", "access"}) {
            const auto& tag = tags.get(key, "");
            if (tag == "private" || tag == "no" || tag == "restricted" || tag == "military" || tag == "emergency") {
                return false;
            }
        }
        const auto& tag = tags.get("highway", "");
        return road_types.contains(tag);
    }
    bool is_oneway(const osm::TagMap& tags) const { return false; }
    NodeAttributes decode_node_tags(const osm::TagMap& tags) const { return NodeAttributes{}; }
    void finalize_node(NodeAttributes& attr, const Coord& geom) const {}
    EdgeAttributes decode_edge_tags(const osm::TagMap& tags) const
    {
        auto str_type = _get_type(tags);
        const auto& maxspeed = tags.get("maxspeed", "");
        const auto& highway = tags.get("highway", "");
        const auto& tracktype = tags.get("tracktype", "");
        const auto& surface = tags.get("surface", "");
        int speed = _get_speed(maxspeed, highway, tracktype, surface);
        return EdgeAttributes{.speed = (int8_t)speed, .type = str_type};
    }
    void finalize_edge(EdgeAttributes& attr, const std::vector<Coord>& geom) const { attr.length = _haversine_length(geom); }
};
