import osmium as osm
import math
from ._pyaccess_ext import Node as GraphNode, Edge as GraphEdge, Coord, CoordVector, NodeVector, EdgeVector, RoadType

class Point:
    __slots__ = "lon", "lat"
    def __init__(self, lon: float, lat: float):
        self.lon: float = lon
        self.lat: float = lat


class OsmNode:
    __slots__ = "point", "count"
    def __init__(self, point: Point, count: int):
        self.point: Point = point
        self.count: int = count


class Node:
    __slots__ = "point", "type", "edges"
    def __init__(self, point, edges):
        self.point: Point = point
        self.type = 0
        self.edges: list[int] = edges


class Edge():
    __slots__ = "nodeA", "nodeB", "oneway", "type", "templimit", "length", "weight", "nodes"
    def __init__(self):
        self.nodeA: int = 0
        self.nodeB: int = 0
        self.oneway: bool = False
        self.type: RoadType = RoadType.ROAD
        self.templimit: int = 0
        self.length: float = 0
        self.weight = 0
        self.nodes: list[Point] = []


class InitWayHandler(osm.SimpleHandler):
    def __init__(self, osm_nodes: dict[int, OsmNode]):
        super(InitWayHandler, self).__init__()
        self.i = 0
        self.osm_nodes: dict[int, OsmNode] = osm_nodes
        self.types = { "motorway","motorway_link","trunk","trunk_link",
        "primary","primary_link","secondary","secondary_link","tertiary","tertiary_link",
        "residential","living_street","service","track", "unclassified", "road" }

    def way(self, w):
        if "highway" not in w.tags:
            return
        if w.tags.get("highway") not in self.types:
            return
        l = len(w.nodes)
        for i in range(0,l):
            ndref = w.nodes[i].ref
            if ndref not in self.osm_nodes:
                self.osm_nodes[ndref] = OsmNode(Point(0,0), 1)
            else:
                self.osm_nodes[ndref].count += 1
        self.osm_nodes[w.nodes[0].ref].count += 1
        self.osm_nodes[w.nodes[-1].ref].count += 1


class NodeHandler(osm.SimpleHandler):
    def __init__(self, osm_nodes: dict[int,OsmNode], nodes: list[Node], index_mapping: dict[int,int]):
        super(NodeHandler, self).__init__()
        self.osm_nodes: dict[int, OsmNode] = osm_nodes
        self.nodes: list[Node] = nodes
        self.index_mapping: dict[int, int] = index_mapping
        self.i = 0
        self.c = 0

    def node(self, n):
        if n.id not in self.osm_nodes:
            return
        self.c += 1
        if self.c % 1000 == 0:
            print(self.c)
        on = self.osm_nodes[n.id]
        if on.count > 1:
            node = Node(Point(n.location.lon, n.location.lat), [])
            self.nodes.append(node)
            self.index_mapping[n.id] = self.i
            self.i += 1
        on.point.lon = n.location.lon
        on.point.lat = n.location.lat


class WayHandler(osm.SimpleHandler):
    def __init__(self, edges: list[Edge], osm_nodes: dict[int,OsmNode], index_mapping: dict[int,int]):
        super(WayHandler, self).__init__()
        self.edges: list[Edge] = edges
        self.osm_nodes: dict[int, OsmNode] = osm_nodes
        self.index_mapping: dict[int, int] = index_mapping
        self.types = { "motorway","motorway_link","trunk","trunk_link",
        "primary","primary_link","secondary","secondary_link","tertiary","tertiary_link",
        "residential","living_street","service","track", "unclassified", "road" }
        self.c = 0

    def way(self, w):
        if "highway" not in w.tags:
            return
        if w.tags.get("highway") not in self.types:
            return
        self.c += 1
        if self.c % 1000 == 0:
            print(self.c)

        start = w.nodes[0].ref
        end = w.nodes[-1].ref
        curr = 0
        e = Edge()
        l = len(w.nodes)
        for i in range(0,l):
            curr = w.nodes[i].ref
            on = self.osm_nodes[curr]
            e.nodes.append(on.point)
            if on.count > 1 and curr != start:
                templimit = w.tags.get("maxspeed", "")
                str_type = w.tags.get("highway")
                oneway = w.tags.get("oneway", "")
                e.templimit = _get_templimit(templimit, str_type)
                e.type = _get_type(str_type)
                e.oneway = _is_oneway(oneway, e.type)
                e.nodeA = self.index_mapping[start]
                e.nodeB = self.index_mapping[curr]
                self.edges.append(e)
                start = curr
                e = Edge()
                e.nodes.append(on.point)


def _parse_osm(file: str, nodes: list[Node], edges: list[Edge], index_mapping: dict[int, int]):
    osm_nodes = {}
    h = InitWayHandler(osm_nodes)
    h.apply_file(file)
    h = NodeHandler(osm_nodes, nodes, index_mapping)
    h.apply_file(file)
    h = WayHandler(edges, osm_nodes, index_mapping)
    h.apply_file(file)
    for i in range(len(edges)):
        e = edges[i]
        nodes[e.nodeA].edges.append(i)
        nodes[e.nodeB].edges.append(i)

def _is_oneway(oneway: str, str_type):
    if str_type == RoadType.MOTORWAY or str_type == RoadType.TRUNK or str_type == RoadType.MOTORWAY_LINK or str_type == RoadType.TRUNK_LINK:
        return True
    if oneway == "yes":
        return True
    return False

def _get_type(type: str):
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

def _get_templimit(templimit:str, streettype:str) -> int:
    if  templimit == "None":
        if (streettype == 'motorway' or streettype == 'trunk'):
            w = 130
        elif (streettype == 'motorway_link' or streettype == 'trunk_link'):
            w = 50
        elif (streettype == 'primary' or streettype == 'secondary'):
            w = 90
        elif (streettype == 'tertiary'):
            w = 70
        elif (streettype == 'primary_link' or streettype == 'secondary_link' or streettype == 'tertiary_link'):
            w = 30
        elif (streettype == 'residential'):
            w = 40
        elif (streettype == 'living_street'):
            w = 10
        else:
            w = 25
    elif templimit == 'walk':
        w = 10
    elif templimit == 'none':
        w = 130
    else:
        try:
            w = int(templimit)
        except:
            w = 20
    return w

def _calc_edge_weights(edges: list[Edge]):
    for e in edges:
        e.length = _haversine_length(e.nodes)
        e.weight = int(e.length * 3.6 / e.templimit)
        if e.weight > 255:
            e.weight = 255

def _haversine_length(points: list[Point], r:float=6365000) -> float:
    """@param geometry: elements must contain lon and lat attribute
    @param r: radius, default Earth
    @returns: length of given geometry
    """
    length = 0
    for i in range (0, len(points)-1):
        lat1 = points[i].lat * math.pi / 180
        lat2 = points[i+1].lat * math.pi / 180
        lon1 = points[i].lon * math.pi / 180
        lon2 = points[i+1].lon * math.pi / 180
        a = math.sin((lat2-lat1)/2)**2
        b = math.sin((lon2-lon1)/2)**2
        length += 2*r*math.asin(math.sqrt(a+math.cos(lat1)*math.cos(lat2)*b))
    return length

def _create_graph_components(nodes: list[Node], edges: list[Edge]):
    graph_nodes = NodeVector()
    graph_edges = EdgeVector()

    for node in nodes:
        g_node = GraphNode()
        g_node.loc = Coord(node.point.lon, node.point.lat)
        graph_nodes.append(g_node)

    for edge in edges:
        graph_edges.append(GraphEdge(edge.nodeA, edge.nodeB, edge.type, edge.length, edge.templimit))
        if not edge.oneway:
            graph_edges.append(GraphEdge(edge.nodeB, edge.nodeA, edge.type, edge.length, edge.templimit))
    
    return graph_nodes, graph_edges

def parse_osm(file: str) -> tuple[NodeVector, EdgeVector]:
    nodes = []
    edges = []
    index_mapping = {}
    _parse_osm(file, nodes, edges, index_mapping)
    print("edges: ", len(edges), ", nodes: ", len(nodes))
    _calc_edge_weights(edges)
    return _create_graph_components(nodes, edges)
