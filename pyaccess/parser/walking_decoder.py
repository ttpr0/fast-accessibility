import math
from typing import Mapping, Any
from shapely import Point, LineString

from .util import haversine_length

road_types = {
    "footway", "path", "steps", "pedestrian", "living_street", "track", "residential", "service",
    "primary", "primary_link", "secondary", "secondary_link", "tertiary", "tertiary_link",
    "cycleway", "unclassified", "road"
}

class WalkingDecoder:
    def __init__(self):
        pass

    def is_valid_highway(self, tags: Mapping[str, str]) -> bool:
        if "highway" not in tags:
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
        return {"speed": 4}

    def finalize_edge(self, attr: dict[str, Any], geom: LineString) -> None:
        attr["length"] = haversine_length(geom)
