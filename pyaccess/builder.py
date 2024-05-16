from typing import Any
from shapely import Point, LineString
import geopandas as gpd
import pandas as pd
import numpy as np

from pyaccess import explorer

from . import _pyaccess_ext
from .graph import Graph, new_graph


#************************************************
# Graph builder
#************************************************

class GraphBuilder:
    _nodes: _pyaccess_ext.NodeVector
    _edges: _pyaccess_ext.EdgeVector
    _node_attr: dict[str, list] | None
    _node_geom: list[Point] | None
    _node_cols: dict[str, Any] | None
    _edge_attr: dict[str, list] | None
    _edge_geom: list[LineString] | None
    _edge_cols: dict[str, Any] | None

    def __init__(self, node_attributes: dict | None = None, edge_attributes: dict | None = None):
        self._nodes = _pyaccess_ext.NodeVector()
        self._edges = _pyaccess_ext.EdgeVector()
        if node_attributes is not None and edge_attributes is not None:
            self._node_cols = node_attributes
            self._node_attr = {attr: [] for attr in self._node_cols}
            self._node_geom = []
            self._edge_cols = edge_attributes
            self._edge_attr = {attr: [] for attr in self._edge_cols}
            self._edge_geom = []
        else:
            self._node_cols = None
            self._node_attr = None
            self._node_geom = None
            self._edge_cols = None
            self._edge_attr = None
            self._edge_geom = None

    def add_node(self, location: Point | tuple[float, float], attributes: dict | None = None) -> int:
        point = location if isinstance(location, Point) else Point(location[0], location[1])
        node = _pyaccess_ext.Node()
        node.loc = _pyaccess_ext.Coord(point.x, point.y)
        self._nodes.append(node)
        if self._node_cols is not None and self._node_attr is not None and self._node_geom is not None:
            if attributes is None:
                raise ValueError("missing attributes")
            for attr in self._node_cols:
                self._node_attr[attr].append(attributes[attr] if attr in attributes else None)
            self._node_geom.append(point)
        return len(self._nodes) - 1

    # def get_node(self, id: int) -> _pyaccess_ext.Node:
    #     return self._nodes[id]

    def add_edge(self, node_a: int, node_b: int, geometry: LineString | None = None, attributes: dict | None = None) -> int:
        if node_a >= len(self._nodes):
            raise ValueError(f"invalid node id: {node_a}")
        if node_b >= len(self._nodes):
            raise ValueError(f"invalid node id: {node_b}")
        edge = _pyaccess_ext.Edge(node_a, node_b)
        self._edges.append(edge)
        if self._edge_cols is not None and self._edge_attr is not None and self._edge_geom is not None:
            if attributes is None or geometry is None:
                raise ValueError("missing attributes or geometry")
            for attr in self._edge_cols:
                self._edge_attr[attr].append(attributes[attr] if attr in attributes else None)
            self._edge_geom.append(geometry)
        return len(self._edges) - 1

    # def get_edge(self, id: int) -> _pyaccess_ext.Edge:
    #     return self._edges[id]

    def build_graph(self) -> Graph:
        node_attr = None
        edge_attr = None
        if self._node_cols is not None and self._node_attr is not None and self._node_geom is not None:
            data = {}
            for col, dtype in self._node_cols.items():
                if isinstance(dtype, tuple) or isinstance(dtype, list):
                    dtype = pd.CategoricalDtype(dtype) # type: ignore
                data[col] = pd.Series(self._node_attr[col], dtype=dtype)
            df = pd.DataFrame(data)
            gs = gpd.GeoSeries(self._node_geom, crs="EPSG:4326") # type: ignore
            node_attr = gpd.GeoDataFrame(df, geometry=gs) # type: ignore
        if self._edge_cols is not None and self._edge_attr is not None and self._edge_geom is not None:
            data = {}
            for col, dtype in self._edge_cols.items():
                if isinstance(dtype, tuple) or isinstance(dtype, list):
                    dtype = pd.CategoricalDtype(dtype) # type: ignore
                data[col] = pd.Series(self._edge_attr[col], dtype=dtype)
            df = pd.DataFrame(data)
            gs = gpd.GeoSeries(self._edge_geom, crs="EPSG:4326") # type: ignore
            edge_attr = gpd.GeoDataFrame(df, geometry=gs) # type: ignore
        return new_graph(self._nodes, self._edges, node_attr, edge_attr)

#************************************************
# Weighting builders
#************************************************

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

def new_transit_weighting(graph: Graph, transit: str) -> _pyaccess_ext.TransitWeighting:
    """Creates a new transit weighting for the given transit-overlay of the graph.

    Schedules are empty by default.
    """
    data = graph._get_transit(transit)
    return _pyaccess_ext.new_transit_weighting(data)

#************************************************
# Weighting builders
#************************************************

def build_fastest_weighting(graph: Graph) -> _pyaccess_ext.Weighting:
    """Build a weighting that uses traversal time of each edge.

    Note:
        This is function only applies for graphs build with a default profile. For custom decoders this function might not work as expected.
    """
    weight = new_weighting(graph)
    explorer = graph.get_explorer()
    for i in range(explorer.edge_count()):
        speed = explorer.get_edge_attribute(i, "speed")
        length = explorer.get_edge_attribute(i, "length")
        weight.set_edge_weight(i, int(length * 3.6 / speed))
    return weight

def build_shortest_weighting(graph: Graph) -> _pyaccess_ext.Weighting:
    """Build a weighting that uses the length of each edge.

    Note:
        This is function only applies for graphs build with a default profile. For custom decoders this function might not work as expected.
    """
    weight = new_weighting(graph)
    explorer = graph.get_explorer()
    for i in range(explorer.edge_count()):
        length = explorer.get_edge_attribute(i, "length")
        weight.set_edge_weight(i, int(length))
    return weight
