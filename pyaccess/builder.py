from shapely import Point, LineString

from . import _pyaccess_ext
from .graph import Graph, new_graph
from .components.base import BaseObject_new

#************************************************
# Graph builder
#************************************************

class GraphBuilder:
    _nodes: _pyaccess_ext.NodeVector
    _edges: _pyaccess_ext.EdgeVector

    def __init__(self):
        self._nodes = _pyaccess_ext.NodeVector()
        self._edges = _pyaccess_ext.EdgeVector()

    def add_node(self, location: Point | tuple[float, float]) -> int:
        point = location if isinstance(location, Point) else Point(location[0], location[1])
        node = _pyaccess_ext.Node()
        node.loc = _pyaccess_ext.Coord(point.x, point.y)
        self._nodes.append(node)
        return len(self._nodes) - 1

    # def get_node(self, id: int) -> _pyaccess_ext.Node:
    #     return self._nodes[id]

    def add_edge(self, node_a: int, node_b: int) -> int:
        if node_a >= len(self._nodes):
            raise ValueError(f"invalid node id: {node_a}")
        if node_b >= len(self._nodes):
            raise ValueError(f"invalid node id: {node_b}")
        edge = _pyaccess_ext.Edge(node_a, node_b)
        self._edges.append(edge)
        return len(self._edges) - 1

    # def get_edge(self, id: int) -> _pyaccess_ext.Edge:
    #     return self._edges[id]

    def build_graph(self) -> Graph:
        base = BaseObject_new(self._nodes, self._edges)
        return Graph(None, None, base, {}, {}, {}, {}, {})
