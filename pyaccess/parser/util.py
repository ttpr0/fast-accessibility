import math
from shapely import LineString

def haversine_length(points: LineString, r: float = 6365000) -> float:
    """Calculate the length of given geometry on a sphere.

    Args:
        points: edge geometry
        r: radius, default Earth

    Returns:
        length of given geometry
    """
    length = 0
    for i in range (0, len(points.coords)-1):
        lat1 = points.coords[i][1] * math.pi / 180
        lat2 = points.coords[i+1][1] * math.pi / 180
        lon1 = points.coords[i][0] * math.pi / 180
        lon2 = points.coords[i+1][0] * math.pi / 180
        a = math.sin((lat2-lat1)/2)**2
        b = math.sin((lon2-lon1)/2)**2
        length += 2*r*math.asin(math.sqrt(a+math.cos(lat1)*math.cos(lat2)*b))
    return length
