import pandas as pd
import numpy as np

from .._pyaccess_ext import NodeVector, EdgeVector, parse_graph_from_osm, nodes_from_edges
from ..graph import Graph
from ..components.base import BaseObject_new


def parse_osm_fast(file: str, profile: str = "driving") -> tuple[Graph, pd.DataFrame, pd.DataFrame]:
    nodes, edges, speed, length, streettype = parse_graph_from_osm(file, profile)
    node_a, node_b = nodes_from_edges(edges)
    base = BaseObject_new(nodes, edges)
    graph = Graph(None, None, base, {}, {}, {}, {}, {})
    nodes_df = pd.DataFrame({"type": np.zeros(len(nodes))})
    edges_df = pd.DataFrame({"node_a": node_a, "node_b": node_b, "speed": speed, "length": length, "streettype": streettype})
    return graph, nodes_df, edges_df
