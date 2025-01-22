import geopandas as gpd
import pandas as pd
import numpy as np

from . import _pyaccess_ext

#************************************************
# Dataframe utlities
#************************************************

def reorder_graph_data(nodes: gpd.GeoDataFrame | pd.DataFrame, edges: gpd.GeoDataFrame | pd.DataFrame, mapping: _pyaccess_ext.IntVector):
    """Reorders nodes and edges by id.
    """
    nodes["_sorter"] = mapping
    nodes.sort_values("_sorter", inplace=True, ignore_index=True)
    del nodes["_sorter"]

def remove_graph_nodes(nodes: gpd.GeoDataFrame | pd.DataFrame, edges: gpd.GeoDataFrame | pd.DataFrame, remove_nodes: _pyaccess_ext.IntVector):
    node_count = nodes.shape[0]
    edge_count = edges.shape[0]
    # get all edges that connect to the removed nodes
    remove_edges = _pyaccess_ext.edges_with_nodes(remove_nodes, edges["node_a"].to_numpy(dtype=np.int32), edges["node_b"].to_numpy(dtype=np.int32))
    # update dataframes
    nodes.drop(remove_nodes, inplace=True)
    nodes.reset_index(drop=True, inplace=True)
    edges.drop(remove_edges, inplace=True)
    edges.reset_index(drop=True, inplace=True)
