import numpy as np
import pandas as pd
import geopandas as gpd
from shapely import Point

from . import _pyaccess_ext
from .graph import Graph


#************************************************
# Utilities to create graphs
#************************************************

def _build_graph(graph: Graph, weight: str) -> _pyaccess_ext.Graph | _pyaccess_ext.TCGraph:
    b = graph._get_base()
    w = graph._get_weight(weight)
    if isinstance(w, _pyaccess_ext.Weighting):
        g = _pyaccess_ext.build_base_graph(b, w)
    elif isinstance(w, _pyaccess_ext.TCWeighting):
        g = _pyaccess_ext.build_tc_graph(b, w)
    else:
        raise ValueError("invalid weighting")
    return g

def _build_ch_graph(graph: Graph, ch: str) -> _pyaccess_ext.CHGraph:
    b = graph._get_base()
    w = graph._get_weight(graph._get_ch_weight(ch))
    if not isinstance(w, _pyaccess_ext.Weighting):
        raise ValueError("invalid weighting")
    ch_d, ch_i, id_m = graph._get_ch(ch)
    g = _pyaccess_ext.build_ch_graph(b, w, id_m, ch_d, ch_i)
    return g

def _build_ch_graph_2(graph: Graph, ch: str) -> _pyaccess_ext.CHGraph2:
    b = graph._get_base()
    w = graph._get_weight(graph._get_ch_weight(ch))
    if not isinstance(w, _pyaccess_ext.Weighting):
        raise ValueError("invalid weighting specified")
    partition = graph._get_ch_partition(ch)
    if partition is None:
        raise ValueError(f"ch {ch} hasnt been build with partition")
    ch_d, ch_i, id_m = graph._get_ch(ch)
    p = graph._get_partition(partition)
    g = _pyaccess_ext.build_ch_graph_2(b, w, p, id_m, ch_d, ch_i)
    return g

def _build_overlay_graph(graph: Graph, overlay: str) -> _pyaccess_ext.TiledGraph:
    b = graph._get_base()
    w = graph._get_weight(graph._get_overlay_weight(overlay))
    if not isinstance(w, _pyaccess_ext.Weighting):
        raise ValueError("invalid weighting specified")
    t_d, t_i, id_m = graph._get_overlay(overlay)
    p = graph._get_partition(graph._get_overlay_partition(overlay))
    g = _pyaccess_ext.build_tiled_graph(b, w, p, id_m, t_d, t_i)
    return g

def _build_transit_graph(graph: Graph, transit: str, transit_weight: str) -> _pyaccess_ext.TransitGraph:
    b = graph._get_base()
    w = graph._get_weight(graph._get_transit_base_weight(transit))
    if isinstance(w, _pyaccess_ext.Weighting):
        g = _pyaccess_ext.build_base_graph(b, w)
    else:
        g = _pyaccess_ext.build_tc_graph(b, w)
    t_d = graph._get_transit(transit)
    t_w = graph._get_transit_weighting(transit, transit_weight)
    tg = _pyaccess_ext.build_transit_graph(g, t_d, t_w)
    return tg


#************************************************
# Input type transformer
#************************************************

def transform_location(location: tuple[float, float] | np.ndarray | Point) -> tuple[float, float]:
    """Transforms input types to tuple of lon and lat arrays.
    """
    if isinstance(location, tuple):
        return location
    elif isinstance(location, np.ndarray):
        location = location.reshape((-1,))
        return location[0], location[1]
    elif isinstance(location, Point):
        return location.x, location.y
    else:
        raise ValueError("Invalid locations type.")

def transform_locations(locations: list[tuple[float, float]] | np.ndarray | tuple[np.ndarray, np.ndarray] | tuple[pd.Series, pd.Series] | gpd.GeoSeries) -> tuple[np.ndarray, np.ndarray]:
    """Transforms input types to tuple of lon and lat arrays.
    """
    if isinstance(locations, list) and isinstance(locations[0], tuple):
        lon = np.zeros((len(locations),), dtype=np.float32)
        lat = np.zeros((len(locations),), dtype=np.float32)
        for i in range(len(locations)):
            lon[i], lat[i] = locations[i]
        return lon, lat
    elif isinstance(locations, tuple) and isinstance(locations[0], pd.Series) and isinstance(locations[1], pd.Series):
        return locations[1].to_numpy(dtype=np.float32), locations[0].to_numpy(dtype=np.float32)
    elif isinstance(locations, gpd.GeoSeries):
        center: gpd.GeoSeries = locations.centroid
        return center.x.to_numpy(dtype=np.float32), center.y.to_numpy(dtype=np.float32)
    elif isinstance(locations, np.ndarray):
        return locations[:,0].astype(np.float32), locations[:,1].astype(np.float32)
    elif isinstance(locations, tuple) and isinstance(locations[0], np.ndarray) and isinstance(locations[1], np.ndarray):
        return locations[0].astype(np.float32), locations[1].astype(np.float32)
    else:
        raise ValueError("Invalid locations type.")

def transform_weights(weights: list[int] | np.ndarray | pd.Series) -> np.ndarray:
    """Transforms input types to numpy arrays.
    """
    if isinstance(weights, list):
        w = np.zeros((len(weights),), dtype=np.int32)
        for i in range(len(weights)):
            w[i] = weights[i]
        return w
    elif isinstance(weights, pd.Series):
        return weights.to_numpy(dtype=np.int32)
    elif isinstance(weights, np.ndarray):
        return weights.astype(np.int32)
    else:
        raise ValueError("Invalid locations type.")
