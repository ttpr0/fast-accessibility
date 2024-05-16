import math
from typing import Mapping, Any
from shapely import Point, LineString

from .util import haversine_length

road_types = {
    "trunk", "trunk_link", "primary", "primary_link", "secondary", "secondary_link", "tertiary", "tertiary_link",
    "unclassified", "living_street", "steps", "path", "residential", "road",
    "service", "footway", "pedestrian", "track", "cycleway",
}

class CyclingDecoder:
    def __init__(self):
        pass

    def is_valid_highway(self, tags: Mapping[str, str]) -> bool:
        if "highway" not in tags:
            return False
        if tags.get("motorroad") == "yes":
            return False
        if tags.get("highway") == "cycleway" or tags.get("bicycle_road") == "yes":
            return True
        if tags.get("bicycle") in ["yes", "designated", "official", "permissive"]:
            return True
        for key in ["bicycle", "vehicle", "access"]:
            tag = tags.get(key)
            if tag in ["private", "no", "restricted", "military", "emergency"]:
                return False
        return tags.get("highway") in road_types

    def is_oneway(self, tags: Mapping[str, str]) -> bool:
        return False

    def get_node_attributes(self) -> dict[str, Any]:
        return {"type": "int8"}

    def decode_node_tags(self, tags: Mapping[str, str]) -> dict[str, Any]:
        return {"type": 0}

    def finalize_node(self, attr: dict[str, Any], geom: Point) -> None:
        return

    def get_edge_attributes(self) -> dict[str, Any]:
        return {
            "speed": "int8",
            "length": "float32",
        }

    def decode_edge_tags(self, tags: Mapping[str, str]) -> dict[str, Any]:
        maxspeed = tags.get("maxspeed", "")
        highway = tags.get("highway", "")
        tracktype = tags.get("tracktype", "")
        surface = tags.get("surface", "")
        speed = _get_speed(maxspeed, highway, tracktype, surface)
        return {
            "speed": speed,
        }

    def finalize_edge(self, attr: dict[str, Any], geom: LineString) -> None:
        attr["length"] = haversine_length(geom)

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
