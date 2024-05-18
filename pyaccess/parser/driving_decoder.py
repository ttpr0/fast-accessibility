from typing import Mapping, Any
from shapely import Point, LineString

from .util import haversine_length

road_types = {
    "motorway","motorway_link","trunk","trunk_link",
    "primary","primary_link","secondary","secondary_link","tertiary","tertiary_link",
    "residential","living_street","service","track", "unclassified", "road", "motorroad"
}

class DrivingDecoder:
    def __init__(self):
        pass

    def is_valid_highway(self, tags: Mapping[str, str]) -> bool:
        if "highway" not in tags:
            return False
        return tags.get("highway") in road_types

    def is_oneway(self, tags: Mapping[str, str]) -> bool:
        str_type = _get_type(tags)
        oneway = _is_oneway(tags, str_type)
        return oneway

    def get_node_attributes(self) -> dict[str, Any]:
        return {"type": "int8"}

    def decode_node_tags(self, tags: Mapping[str, str]) -> dict[str, Any]:
        return {"type": 0}
    
    def finalize_node(self, attr: dict[str, Any], geom: Point) -> None:
        return

    def get_edge_attributes(self) -> dict[str, Any]:
        return {
            "speed": "int8",
            "stree_type": ("motorway", "motorway_link", "trunk", "trunk_link", "primary", "primary_link", "secondary", "secondary_link", "tertiary", "tertiary_link", "residential", "living_street", "unclassified", "road", "track"),
            "length": "float32",
        }

    def decode_edge_tags(self, tags: Mapping[str, str]) -> dict[str, Any]:
        str_type = _get_type(tags)
        speed = _get_speed(tags, str_type)
        return {
            "speed": speed,
            "stree_type": str_type,
        }

    def finalize_edge(self, attr: dict[str, Any], geom: LineString) -> None:
        attr["length"] = haversine_length(geom)

def _is_oneway(tags: Mapping[str, str], str_type: str) -> bool:
    if str_type == "motorway" or str_type == "trunk" or str_type == "motorway_link" or str_type == "trunk_link":
        return True
    oneway = tags.get("oneway", "")
    if oneway == "yes":
        return True
    return False

def _get_type(tags: Mapping[str, str]) -> str:
    type = tags.get("highway", "")
    if type in ["motorway", "motorway_link", "trunk", "trunk_link", "primary", "primary_link", "secondary", "secondary_link", "tertiary", "tertiary_link", "residential", "living_street", "unclassified", "road", "track"]:
        return type
    return "road"

def _get_speed(tags: Mapping[str, str], streettype: str) -> int:
    tracktype = tags.get("tracktype", "")
    surface = tags.get("surface", "")
    speed = 0
    # check if maxspeed is set
    if "maxspeed" in tags or "maxspeed:forward" in tags or "maxspeed:backward" in tags:
        if "maxspeed" in tags:
            templimit = tags.get("maxspeed", "")
        elif "maxspeed:forward" in tags:
            templimit = tags.get("maxspeed:forward", "")
        elif "maxspeed:backward" in tags:
            templimit = tags.get("maxspeed:backward", "")
        else:
            templimit = 20
        if templimit == "walk":
            speed = 10
        elif templimit == "none":
            speed = 110
        else:
            try:
                speed = int(templimit)
            except:
                speed = 20
        speed = int(0.9 * speed)
    elif "zone:maxspeed" in tags:
        try:
            templimit_zone = tags.get("zone:maxspeed", "")
            templimit = templimit_zone.split(":")[1]
            speed = int(templimit)
        except:
            speed = 20
        speed = int(0.9 * speed)
    elif "zone:traffic" in tags:
        try:
            traffic_zone = tags.get("zone:traffic", "")
            zone = traffic_zone.split(":")[1]
            match zone:
                case "motorway": speed = 130
                case "rural": speed = 100
                case "urban": speed = 50
                case "living_street": speed = 10
                case _: speed = 20
        except:
            speed = 20
        speed = int(0.9 * speed)
    else:
        # is templimit not set take maxspeed from streettype
        match streettype:
            case "motorway":
                speed = 100
            case "trunk":
                speed = 85
            case "motorway_link" | "trunk_link":
                speed = 60
            case "primary":
                speed = 65
            case "secondary":
                speed = 60
            case "tertiary":
                speed = 50
            case "primary_link" | "secondary_link":
                speed = 50
            case "tertiary_link":
                speed = 40
            case "unclassified":
                speed = 30
            case "residential":
                speed = 30
            case "living_street":
                speed = 10
            case "road":
                speed = 20
            case "track":
                if tracktype == "":
                    speed = 15
                else:
                    match tracktype:
                        case "grade1":
                            speed = 40
                        case "grade2":
                            speed = 30
                        case "grade3":
                            speed = 20
                        case "grade4":
                            speed = 15
                        case "grade5":
                            speed = 10
                        case _:
                            speed = 15
            case _:
                speed = 20
    # reduce speed to maximum of given surface
    match surface:
        case "cement" | "compacted":
            if speed > 80:
                speed = 80
        case "fine_gravel":
            if speed > 60:
                speed = 60
        case "paving_stones" | "metal" | "bricks":
            if speed > 40:
                speed = 40
        case "grass" | "wood" | "sett" | "grass_paver" | "gravel" | "unpaved" | "ground" | "dirt" | "pebblestone" | "tartan":
            if speed > 30:
                speed = 30
        case "cobblestone" | "clay":
            if speed > 20:
                speed = 20
        case "earth" | "stone" | "rocky" | "sand":
            if speed > 15:
                speed = 15
        case "mud":
            if speed > 10:
                speed = 10
    # if speed is 0 set it to minimum value
    if speed < 0:
        speed = 10
    if speed > 125:
        speed = 125
    return speed
