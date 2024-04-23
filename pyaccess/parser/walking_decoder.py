import math
from typing import Mapping

from .._pyaccess_ext import RoadType

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

    def decode_node(self, tags: Mapping[str, str]) -> int:
        return 0

    def decode_way(self, tags: Mapping[str, str]) -> tuple[int, RoadType, bool]:
        return 4, RoadType.UNCLASSIFIED, False
