

from . import _pyaccess_ext
from .graph import Graph, new_graph

class GraphBuilder:
    _nodes: _pyaccess_ext.NodeVector
    _edges: _pyaccess_ext.EdgeVector

    def __init__(self):
        self._nodes = _pyaccess_ext.NodeVector()
        self._edges = _pyaccess_ext.EdgeVector()

    def add_node(self, location: tuple[float, float]) -> int:
        node = _pyaccess_ext.Node()
        node.loc = _pyaccess_ext.Coord()
        node.loc.lon = location[0]
        node.loc.lat = location[1]
        self._nodes.append(node)
        return len(self._nodes) - 1

    def get_node(self, id: int) -> _pyaccess_ext.Node:
        return self._nodes[id]

    def add_edge(self, node_a: int, node_b: int, length: float, maxspeed: int, typ: _pyaccess_ext.RoadType) -> int:
        if node_a >= len(self._nodes):
            raise ValueError(f"invalid node id: {node_a}")
        if node_b >= len(self._nodes):
            raise ValueError(f"invalid node id: {node_b}")
        edge = _pyaccess_ext.Edge(node_a, node_b, typ, length, maxspeed)
        self._edges.append(edge)
        return len(self._edges) - 1

    def get_edge(self, id: int) -> _pyaccess_ext.Edge:
        return self._edges[id]

    def build_graph(self) -> Graph:
        return new_graph(self._nodes, self._edges)

def new_weighting(graph: Graph) -> _pyaccess_ext.Weighting:
    """Creates a new weighting for the given graph.

    Edge weights are initiallized to 1.
    """
    base = graph._get_base()
    return _pyaccess_ext.new_weighting(base)

def new_tc_weighting(graph: Graph) -> _pyaccess_ext.TCWeighting:
    """Creates a new turn-cost weighting for the given graph.

    Edge weights are initiallized to 1.
    Turn costs are initiallized to 0.
    """
    base = graph._get_base()
    return _pyaccess_ext.new_tc_weighting(base)
