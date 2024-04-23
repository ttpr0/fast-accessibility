import math
from typing import Mapping

from .._pyaccess_ext import RoadType

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

    def decode_node(self, tags: Mapping[str, str]) -> int:
        return 0

    def decode_way(self, tags: Mapping[str, str]) -> tuple[int, RoadType, bool]:
        str_type = _get_type(tags)
        speed = _get_speed(tags, str_type)
        oneway = _is_oneway(tags, str_type)
        return speed, str_type, oneway

def _is_oneway(tags: Mapping[str, str], str_type: RoadType) -> bool:
    if str_type == RoadType.MOTORWAY or str_type == RoadType.TRUNK or str_type == RoadType.MOTORWAY_LINK or str_type == RoadType.TRUNK_LINK:
        return True
    oneway = tags.get("oneway", "")
    if oneway == "yes":
        return True
    return False

def _get_type(tags: Mapping[str, str]) -> RoadType:
    type = tags.get("highway", "")
    if type == "motorway": return RoadType.MOTORWAY
    if type == "motorway_link": return RoadType.MOTORWAY_LINK
    if type == "trunk": return RoadType.TRUNK
    if type == "trunk_link": return RoadType.TRUNK_LINK
    if type == "primary": return RoadType.PRIMARY
    if type == "primary_link": return RoadType.PRIMARY_LINK
    if type == "secondary": return RoadType.SECONDARY
    if type == "secondary_link": return RoadType.SECONDARY_LINK
    if type == "tertiary": return RoadType.TERTIARY
    if type == "tertiary_link": return RoadType.TERTIARY_LINK
    if type == "residential": return RoadType.RESIDENTIAL
    if type == "living_street": return RoadType.LIVING_STREET
    if type == "unclassified": return RoadType.UNCLASSIFIED
    if type == "road": return RoadType.ROAD
    if type == "track": return RoadType.TRACK
    return RoadType.ROAD

def _get_speed(tags: Mapping[str, str], streettype: RoadType) -> int:
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
        templimit_zone = tags.get("zone:maxspeed", "")
        templimit = templimit_zone.split(":")[1]
        try:
            speed = int(templimit)
        except:
            speed = 20
        speed = int(0.9 * speed)
    elif "zone:traffic" in tags:
        traffic_zone = tags.get("zone:traffic", "")
        zone = traffic_zone.split(":")[1]
        try:
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
            case RoadType.MOTORWAY:
                speed = 100
            case RoadType.TRUNK:
                speed = 85
            case RoadType.MOTORWAY_LINK, RoadType.TRUNK_LINK:
                speed = 60
            case RoadType.PRIMARY:
                speed = 65
            case RoadType.SECONDARY:
                speed = 60
            case RoadType.TERTIARY:
                speed = 50
            case RoadType.PRIMARY_LINK, RoadType.SECONDARY_LINK:
                speed = 50
            case RoadType.TERTIARY_LINK:
                speed = 40
            case RoadType.UNCLASSIFIED:
                speed = 30
            case RoadType.RESIDENTIAL:
                speed = 30
            case RoadType.LIVING_STREET:
                speed = 10
            case RoadType.ROAD:
                speed = 20
            case RoadType.TRACK:
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
        case "cement", "compacted":
            if speed > 80:
                speed = 80
        case "fine_gravel":
            if speed > 60:
                speed = 60
        case "paving_stones", "metal", "bricks":
            if speed > 40:
                speed = 40
        case "grass", "wood", "sett", "grass_paver", "gravel", "unpaved", "ground", "dirt", "pebblestone", "tartan":
            if speed > 30:
                speed = 30
        case "cobblestone", "clay":
            if speed > 20:
                speed = 20
        case "earth", "stone", "rocky", "sand":
            if speed > 15:
                speed = 15
        case "mud":
            if speed > 10:
                speed = 10
    # if speed is 0 set it to minimum value
    if speed == 0:
        speed = 10
    return speed
