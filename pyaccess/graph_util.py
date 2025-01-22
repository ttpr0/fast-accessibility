from __future__ import annotations
from typing import Any
import os
import json
import numpy as np
import geopandas as gpd
import pandas as pd

from .explorer import Explorer
from . import _pyaccess_ext
from .graph import Graph, new_weighting

#************************************************
# Utilities
#************************************************

def calc_dfs_ordering(graph: Graph) -> _pyaccess_ext.IntVector:
    """Computes the DFS-ordering of the graph.

    Returns:
        Mapping from current id to DFS-order id.
    """
    base = graph._get_base()
    return _pyaccess_ext.calc_dfs_order(base)

def calc_unconnected_nodes(graph: Graph) -> _pyaccess_ext.IntVector:
    """Computes all nodes not fully connected to the largest connected component.
    """
    base = graph._get_base()
    return _pyaccess_ext.calc_unconnected(base)

#************************************************
# Weighting builders
#************************************************

def build_fastest_weighting(graph: Graph, edges: gpd.GeoDataFrame | pd.DataFrame) -> _pyaccess_ext.Weighting:
    """Build a weighting that uses traversal time of each edge.

    Note:
        This is function only applies for graphs build with a default profile. For custom decoders this function might not work as expected.
    """
    weight = new_weighting(graph)
    speed = edges["speed"].to_numpy(dtype=np.float32)
    length = edges["length"].to_numpy(dtype=np.float32)
    weight.set_edge_weights(length*3.6 / speed)
    return weight

def build_shortest_weighting(graph: Graph, edges: gpd.GeoDataFrame | pd.DataFrame) -> _pyaccess_ext.Weighting:
    """Build a weighting that uses the length of each edge.

    Note:
        This is function only applies for graphs build with a default profile. For custom decoders this function might not work as expected.
    """
    weight = new_weighting(graph)
    length = edges["length"].to_numpy(dtype=np.float32)
    weight.set_edge_weights(length)
    return weight
