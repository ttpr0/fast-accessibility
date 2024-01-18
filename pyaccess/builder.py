

from . import _pyaccess_ext
from .graph import Graph, new_graph

class GraphBuilder:
    nodes: _pyaccess_ext.NodeVector
    edges: _pyaccess_ext.EdgeVector

    def __init__(self):
        self.nodes = _pyaccess_ext.NodeVector()
        self.edges = _pyaccess_ext.EdgeVector()

    def add_node(self, location: tuple[float, float]) -> int:
        node = _pyaccess_ext.Node()
        node.loc = _pyaccess_ext.Coord()
        node.loc.lon = location[0]
        node.loc.lat = location[1]
        self.nodes.append(node)
        return len(self.nodes) - 1

    def get_node(self, id: int) -> _pyaccess_ext.Node:
        return self.nodes[id]

    def add_edge(self, node_a: int, node_b: int, length: float, maxspeed: int, typ: _pyaccess_ext.RoadType) -> int:
        if node_a >= len(self.nodes):
            raise ValueError(f"invalid node id: {node_a}")
        if node_b >= len(self.nodes):
            raise ValueError(f"invalid node id: {node_b}")
        edge = _pyaccess_ext.Edge(node_a, node_b, typ, length, maxspeed)
        self.edges.append(edge)
        return len(self.edges) - 1

    def get_edge(self, id: int) -> _pyaccess_ext.Edge:
        return self.edges[id]

    def build_graph(self) -> Graph:
        return new_graph(self.nodes, self.edges)

def new_weighting(graph: Graph) -> _pyaccess_ext.Weighting:
    base = graph._get_base()
    return _pyaccess_ext.prepare_default_weighting(base)

def new_tc_weighting(graph: Graph) -> _pyaccess_ext.TCWeighting:
    base = graph._get_base()
    return _pyaccess_ext.prepare_default_tc_weighting(base)
