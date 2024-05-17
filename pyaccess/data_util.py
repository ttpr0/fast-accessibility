import geopandas as gpd

from . import _pyaccess_ext

#************************************************
# Dataframe utlities
#************************************************

def reorder_graph_data(nodes: gpd.GeoDataFrame, edges: gpd.GeoDataFrame, mapping: _pyaccess_ext.IntVector):
    """Reorders nodes and edges by id.
    """
    nodes["_sorter"] = mapping
    nodes.sort_values("_sorter", inplace=True, ignore_index=True)
    del nodes["_sorter"]

def remove_graph_nodes(nodes: gpd.GeoDataFrame, edges: gpd.GeoDataFrame, remove_nodes: _pyaccess_ext.IntVector):
    node_count = nodes.shape[0]
    edge_count = edges.shape[0]
    # get all edges that connect to the removed nodes
    node_set = set(remove_nodes)
    remove_edges = []
    for i in range(edge_count):
        node_a = edges["node_a"][i]
        node_b = edges["node_b"][i]
        if node_a in node_set or node_b in node_set:
            remove_edges.append(i)
    # update dataframes
    nodes.drop(remove_nodes, inplace=True)
    nodes.reset_index(drop=True, inplace=True)
    edges.drop(remove_edges, inplace=True)
    edges.reset_index(drop=True, inplace=True)
