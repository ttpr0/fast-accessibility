from . import _pyaccess_ext
from .graph import Graph

def _build_graph(graph: Graph, weight: str) -> _pyaccess_ext.Graph | _pyaccess_ext.TCGraph:
    b = graph._get_base()
    w = graph._get_weight(weight)
    i = graph._get_index()
    if isinstance(w, _pyaccess_ext.Weighting):
        g = _pyaccess_ext.build_base_graph(b, w, i)
    elif isinstance(w, _pyaccess_ext.TCWeighting):
        g = _pyaccess_ext.build_tc_graph(b, w, i)
    else:
        raise ValueError("invalid weighting")
    return g

def _build_ch_graph(graph: Graph, ch: str) -> _pyaccess_ext.CHGraph:
    b = graph._get_base()
    w = graph._get_weight(graph._get_ch_weight(ch))
    if not isinstance(w, _pyaccess_ext.Weighting):
        raise ValueError("invalid weighting")
    i = graph._get_index()
    ch_d, ch_i, id_m = graph._get_ch(ch)
    g = _pyaccess_ext.build_ch_graph(b, w, i, id_m, ch_d, ch_i)
    return g

def _build_ch_graph_2(graph: Graph, ch: str) -> _pyaccess_ext.CHGraph2:
    b = graph._get_base()
    w = graph._get_weight(graph._get_ch_weight(ch))
    if not isinstance(w, _pyaccess_ext.Weighting):
        raise ValueError("invalid weighting specified")
    i = graph._get_index()
    partition = graph._get_ch_partition(ch)
    if partition is None:
        raise ValueError(f"ch {ch} hasnt been build with partition")
    ch_d, ch_i, id_m = graph._get_ch(ch)
    p = graph._get_partition(partition)
    g = _pyaccess_ext.build_ch_graph_2(b, w, i, p, id_m, ch_d, ch_i)
    return g

def _build_overlay_graph(graph: Graph, overlay: str) -> _pyaccess_ext.TiledGraph:
    b = graph._get_base()
    w = graph._get_weight(graph._get_overlay_weight(overlay))
    if not isinstance(w, _pyaccess_ext.Weighting):
        raise ValueError("invalid weighting specified")
    i = graph._get_index()
    t_d, t_i, id_m = graph._get_overlay(overlay)
    p = graph._get_partition(graph._get_overlay_partition(overlay))
    g = _pyaccess_ext.build_tiled_graph(b, w, i, p, id_m, t_d, t_i)
    return g
