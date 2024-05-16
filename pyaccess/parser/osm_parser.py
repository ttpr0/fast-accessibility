import osmium as osm
from typing import Protocol, Mapping, Any
from shapely import Point, LineString

from .driving_decoder import DrivingDecoder
from .walking_decoder import WalkingDecoder
from .cycling_decoder import CyclingDecoder
from .._pyaccess_ext import Node as GraphNode, Edge as GraphEdge, Coord, CoordVector, NodeVector, EdgeVector
from ..graph import Graph
from ..builder import GraphBuilder


#***************************************************
# decoder interface
#***************************************************

class IOSMDecoder(Protocol):
    def is_valid_highway(self, tags: Mapping[str, str]) -> bool:
        """Checks if the road is valid for the profile used.
        """
        ...
    def is_oneway(self, tags: Mapping[str, str]) -> bool:
        """Checks if the road is oneway.
        """
        ...
    def get_node_attributes(self) -> dict[str, Any]:
        """Returns node attributes tags will be decoded to.

        Returns:
            node attributes as dictionary name->dtype
        """
        ...
    def decode_node_tags(self, tags: Mapping[str, str]) -> dict[str, Any]:
        """Decodes node attributes from osm tags.

        Args:
            tags: osm tags

        Returns:
            node attributes
        """
        ...
    def finalize_node(self, attr: dict[str, Any], geom: Point) -> None:
        """Finalizes node attributes after decoding.
        """
        ...
    def get_edge_attributes(self) -> dict[str, Any]:
        """Returns edge attributes tags will be decoded to.

        Returns:
            edge attributes as dictionary name->dtype
        """
        ...
    def decode_edge_tags(self, tags: Mapping[str, str]) -> dict[str, Any]:
        """Decodes edge attributes from osm tags.

        Args:
            tags: osm tags

        Returns:
            edge attributes
        """
        ...
    def finalize_edge(self, attr: dict[str, Any], geom: LineString) -> None:
        """Finalizes edge attributes after decoding.
        """
        ...

#***************************************************
# utility classes
#***************************************************

class OsmNode:
    __slots__ = "point", "count"
    def __init__(self, point: Coord, count: int):
        self.point: Coord = point
        self.count: int = count


class Node:
    __slots__ = "point", "attr", "edges"
    def __init__(self, point: Coord, attr: dict, edges: list[int]):
        self.point = point
        self.attr = attr
        self.edges = edges


class Edge():
    __slots__ = "nodeA", "nodeB", "oneway", "attr", "points"
    def __init__(self):
        self.nodeA: int = 0
        self.nodeB: int = 0
        self.oneway: bool = False
        self.attr: dict = {}
        self.points = CoordVector()

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
                self.osm_nodes[ndref] = OsmNode(Coord(0.0,0.0), 1)
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
            attr = self.decoder.decode_node_tags(n.tags)
            node = Node(Coord(n.location.lon, n.location.lat), attr, [])
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
            e.points.append(on.point)
            if on.count > 1 and curr != start:
                e.attr = self.decoder.decode_edge_tags(w.tags)
                e.oneway = self.decoder.is_oneway(w.tags)
                e.nodeA = self.index_mapping[start]
                e.nodeB = self.index_mapping[curr]
                self.edges.append(e)
                start = curr
                e = Edge()
                e.points.append(on.point)

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

def _create_graph(nodes: list[Node], edges: list[Edge], decoder: IOSMDecoder) -> Graph:
    builder = GraphBuilder(decoder.get_node_attributes(), decoder.get_edge_attributes())

    for node in nodes:
        geom = Point(node.point.lon, node.point.lat)
        decoder.finalize_node(node.attr, geom)
        builder.add_node(geom, node.attr)

    for edge in edges:
        geom = LineString([(edge.points[i].lon, edge.points[i].lat) for i in range(len(edge.points))])
        decoder.finalize_edge(edge.attr, geom)
        builder.add_edge(edge.nodeA, edge.nodeB, geom, edge.attr)
        if not edge.oneway:
            builder.add_edge(edge.nodeB, edge.nodeA, geom, edge.attr)
    
    return builder.build_graph()

def parse_osm(file: str, profile: str | IOSMDecoder = "driving") -> Graph:
    nodes = []
    edges = []
    index_mapping = {}
    if not isinstance(profile, str):
        decoder = profile
    else:
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
    graph = _create_graph(nodes, edges, decoder)
    return graph
