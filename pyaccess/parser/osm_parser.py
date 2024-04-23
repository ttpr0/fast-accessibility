import osmium as osm
import math
from typing import Protocol, Mapping

from .driving_decoder import DrivingDecoder
from .walking_decoder import WalkingDecoder
from .cycling_decoder import CyclingDecoder
from .._pyaccess_ext import Node as GraphNode, Edge as GraphEdge, Coord, CoordVector, NodeVector, EdgeVector, RoadType
from ..graph import Graph, new_graph


#***************************************************
# decoder interface
#***************************************************

class IOSMDecoder(Protocol):
    def is_valid_highway(self, tags: Mapping[str, str]) -> bool:
        """Checks if the road is valid for the profile used.
        """
        ...
    def decode_node(self, tags: Mapping[str, str]) -> int:
        """Decodes node attributes from osm tags.

        Args:
            tags: osm tags

        Returns:
            nodetype
        """
        ...
    def decode_way(self, tags: Mapping[str, str]) -> tuple[int, RoadType, bool]:
        """Decodes way attributes from osm tags.

        Args:
            tags: osm tags

        Returns:
            (speed, roadtype, oneway)
        """
        ...

#***************************************************
# utility classes
#***************************************************

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
    __slots__ = "nodeA", "nodeB", "oneway", "type", "templimit", "length", "nodes"
    def __init__(self):
        self.nodeA: int = 0
        self.nodeB: int = 0
        self.oneway: bool = False
        self.type: RoadType = RoadType.ROAD
        self.templimit: int = 0
        self.length: float = 0
        self.nodes: list[Point] = []

#***************************************************
# osm handlers
#***************************************************

class InitWayHandler(osm.SimpleHandler):
    def __init__(self, decoder: IOSMDecoder, osm_nodes: dict[int, OsmNode]):
        super(InitWayHandler, self).__init__()
        self.decoder = decoder
        self.osm_nodes: dict[int, OsmNode] = osm_nodes
        self.i = 0

    def way(self, w):
        if not self.decoder.is_valid_highway(w.tags):
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
    def __init__(self, decoder: IOSMDecoder, osm_nodes: dict[int,OsmNode], nodes: list[Node], index_mapping: dict[int,int]):
        super(NodeHandler, self).__init__()
        self.decoder = decoder
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
            type_ = self.decoder.decode_node(n.tags)
            node = Node(Point(n.location.lon, n.location.lat), [])
            self.nodes.append(node)
            self.index_mapping[n.id] = self.i
            self.i += 1
        on.point.lon = n.location.lon
        on.point.lat = n.location.lat


class WayHandler(osm.SimpleHandler):
    def __init__(self, decoder: IOSMDecoder, edges: list[Edge], osm_nodes: dict[int,OsmNode], index_mapping: dict[int,int]):
        super(WayHandler, self).__init__()
        self.decoder = decoder
        self.edges: list[Edge] = edges
        self.osm_nodes: dict[int, OsmNode] = osm_nodes
        self.index_mapping: dict[int, int] = index_mapping
        self.c = 0

    def way(self, w):
        if not self.decoder.is_valid_highway(w.tags):
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
                e.templimit, e.type, e.oneway = self.decoder.decode_way(w.tags)
                e.nodeA = self.index_mapping[start]
                e.nodeB = self.index_mapping[curr]
                self.edges.append(e)
                start = curr
                e = Edge()
                e.nodes.append(on.point)

#***************************************************
# parser functions
#***************************************************

def _parse_osm(file: str, decoder: IOSMDecoder, nodes: list[Node], edges: list[Edge], index_mapping: dict[int, int]):
    osm_nodes = {}
    h = InitWayHandler(decoder, osm_nodes)
    h.apply_file(file)
    h = NodeHandler(decoder, osm_nodes, nodes, index_mapping)
    h.apply_file(file)
    h = WayHandler(decoder, edges, osm_nodes, index_mapping)
    h.apply_file(file)
    for i in range(len(edges)):
        e = edges[i]
        nodes[e.nodeA].edges.append(i)
        nodes[e.nodeB].edges.append(i)

def _calc_edge_weights(edges: list[Edge]):
    for e in edges:
        e.length = _haversine_length(e.nodes)

def _haversine_length(points: list[Point], r: float = 6365000) -> float:
    """Calculate the length of given geometry on a sphere.

    Args:
        points: elements must contain lon and lat attribute
        r: radius, default Earth

    Returns:
        length of given geometry
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

def parse_osm(file: str, profile: str = "driving") -> Graph:
    nodes = []
    edges = []
    index_mapping = {}
    match profile:
        case "driving":
            decoder = DrivingDecoder()
        case "walking":
            decoder = WalkingDecoder()
        case "cycling":
            decoder = CyclingDecoder()
        case _:
            raise ValueError("unknown profile: " + profile)
    _parse_osm(file, decoder, nodes, edges, index_mapping)
    print("edges: ", len(edges), ", nodes: ", len(nodes))
    _calc_edge_weights(edges)
    node_vec, edge_vec = _create_graph_components(nodes, edges)
    graph =  new_graph(node_vec, edge_vec)
    graph.add_default_weighting()
    return graph
