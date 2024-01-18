from enum import Enum
from typing import TypeAlias

from . import _pyaccess_ext
from .graph import Graph

class OneToManyType(Enum):
    RANGE_DIJKSTRA = 0
    RANGE_PHAST = 1
    RANGE_RPHAST = 3
    RANGE_RPHAST_GS = 4
    GRASP = 5

OneToManySolver: TypeAlias = _pyaccess_ext.RangeDijkstra | _pyaccess_ext.RangePHAST | _pyaccess_ext.RangeRPHAST

def calc_2sfca(graph: Graph, dem_points: list[tuple[float, float]], dem_weight: list[int], sup_points: list[tuple[float, float]], sup_weight: list[int], decay: _pyaccess_ext.IDistanceDecay = _pyaccess_ext.BinaryDecay(900), algorithm: OneToManyType = OneToManyType.RANGE_DIJKSTRA, weight: str = "default", partition: str | None = None, ch: str | None = None) -> list[float]:
    g: _pyaccess_ext.IGraph

    match algorithm:
        case OneToManyType.RANGE_DIJKSTRA:
            b = graph._get_base()
            w = graph._get_weight(weight)
            if not isinstance(w, _pyaccess_ext.Weighting):
                raise ValueError("invalid weighting specified")
            i = graph._get_index()
            g = _pyaccess_ext.build_base_graph(b, w, i)
            solver = _pyaccess_ext.build_range_dijkstra_solver(g)
        case OneToManyType.RANGE_PHAST:
            b = graph._get_base()
            w = graph._get_weight(weight)
            if not isinstance(w, _pyaccess_ext.Weighting):
                raise ValueError("invalid weighting specified")
            i = graph._get_index()
            if ch is None:
                raise ValueError("no ch specified")
            ch_d, ch_i, id_m = graph._get_ch(ch)
            g = _pyaccess_ext.build_ch_graph(b, w, i, id_m, ch_d, ch_i)
            solver = _pyaccess_ext.build_range_phast_solver(g)
        case OneToManyType.RANGE_RPHAST:
            b = graph._get_base()
            w = graph._get_weight(weight)
            if not isinstance(w, _pyaccess_ext.Weighting):
                raise ValueError("invalid weighting specified")
            i = graph._get_index()
            if ch is None:
                raise ValueError("no ch specified")
            ch_d, ch_i, id_m = graph._get_ch(ch)
            g = _pyaccess_ext.build_ch_graph(b, w, i, id_m, ch_d, ch_i)
            solver = _pyaccess_ext.build_range_rphast_solver(g)
        case OneToManyType.RANGE_RPHAST_GS:
            b = graph._get_base()
            w = graph._get_weight(weight)
            if not isinstance(w, _pyaccess_ext.Weighting):
                raise ValueError("invalid weighting specified")
            i = graph._get_index()
            if ch is None:
                raise ValueError("no ch specified")
            partition = graph._get_ch_partition(ch)
            if partition is None:
                raise ValueError(f"ch {ch} hasnt been build with partition")
            ch_d, ch_i, id_m = graph._get_ch(ch)
            p = graph._get_partition(partition)
            g = _pyaccess_ext.build_ch_graph_2(b, w, i, p, id_m, ch_d, ch_i)
            solver = _pyaccess_ext.build_range_rphast_gs_solver(g)
        case _:
            raise NotImplementedError("")

    dem_nodes = _pyaccess_ext.map_to_closest(dem_points, g)
    sup_nodes = _pyaccess_ext.map_to_closest(sup_points, g)
    dem_weights = _pyaccess_ext.IntVector(dem_weight)
    sup_weights = _pyaccess_ext.IntVector(sup_weight)

    access = _pyaccess_ext.calc_2sfca(solver, dem_nodes, dem_weights, sup_nodes, sup_weights, decay)
    return list(access)

def calc_range(graph: Graph, sup_point: tuple[float, float], dem_points: list[tuple[float, float]], max_range: int = 900, algorithm: OneToManyType = OneToManyType.RANGE_DIJKSTRA, weight: str = "default", ch: str | None = None, overlay: str | None = None) -> list[int]:
    g: _pyaccess_ext.IGraph

    match algorithm:
        case OneToManyType.RANGE_DIJKSTRA:
            b = graph._get_base()
            w = graph._get_weight(weight)
            if not isinstance(w, _pyaccess_ext.Weighting):
                raise ValueError("invalid weighting specified")
            i = graph._get_index()
            g = _pyaccess_ext.build_base_graph(b, w, i)
            solver = _pyaccess_ext.build_range_dijkstra_solver(g)
        case OneToManyType.RANGE_PHAST:
            b = graph._get_base()
            w = graph._get_weight(weight)
            if not isinstance(w, _pyaccess_ext.Weighting):
                raise ValueError("invalid weighting specified")
            i = graph._get_index()
            if ch is None:
                raise ValueError("no ch specified")
            ch_d, ch_i, id_m = graph._get_ch(ch)
            g = _pyaccess_ext.build_ch_graph(b, w, i, id_m, ch_d, ch_i)
            solver = _pyaccess_ext.build_range_phast_solver(g)
        case OneToManyType.RANGE_RPHAST:
            b = graph._get_base()
            w = graph._get_weight(weight)
            if not isinstance(w, _pyaccess_ext.Weighting):
                raise ValueError("invalid weighting specified")
            i = graph._get_index()
            if ch is None:
                raise ValueError("no ch specified")
            ch_d, ch_i, id_m = graph._get_ch(ch)
            g = _pyaccess_ext.build_ch_graph(b, w, i, id_m, ch_d, ch_i)
            solver = _pyaccess_ext.build_range_rphast_solver(g)
        case OneToManyType.GRASP:
            b = graph._get_base()
            w = graph._get_weight(weight)
            if not isinstance(w, _pyaccess_ext.Weighting):
                raise ValueError("invalid weighting specified")
            i = graph._get_index()
            if overlay is None:
                raise ValueError("no overlay specified")
            t_d, t_i, id_m = graph._get_overlay(overlay)
            p = graph._get_partition(graph._get_overlay_partition(overlay))
            g = _pyaccess_ext.build_tiled_graph(b, w, i, p, id_m, t_d, t_i)
            solver = _pyaccess_ext.build_grasp_solver(g)
        case _:
            raise NotImplementedError("")

    dem_nodes = _pyaccess_ext.map_to_closest(dem_points, g)
    sup_node = _pyaccess_ext.map_to_closest(sup_point, g)

    range = _pyaccess_ext.calc_range_query(solver, sup_node, dem_nodes, max_range)
    return list(range)

def calc_reachability(graph: Graph, dem_points: list[tuple[float, float]], sup_points: list[tuple[float, float]], sup_weight: list[int], decay: _pyaccess_ext.IDistanceDecay = _pyaccess_ext.BinaryDecay(900), algorithm: OneToManyType = OneToManyType.RANGE_DIJKSTRA, weight: str = "default") -> list[float]:
    g: _pyaccess_ext.IGraph

    match algorithm:
        case OneToManyType.RANGE_DIJKSTRA:
            b = graph._get_base()
            w = graph._get_weight(weight)
            if not isinstance(w, _pyaccess_ext.Weighting):
                raise ValueError("invalid weighting specified")
            i = graph._get_index()
            g = _pyaccess_ext.build_base_graph(b, w, i)
            solver = _pyaccess_ext.build_range_dijkstra_solver(g)
        case _:
            raise NotImplementedError("")

    dem_nodes = _pyaccess_ext.map_to_closest(dem_points, g)
    sup_nodes = _pyaccess_ext.map_to_closest(sup_points, g)
    sup_weights = _pyaccess_ext.IntVector(sup_weight)

    reachability = _pyaccess_ext.calc_reachability(solver, dem_nodes, sup_nodes, sup_weights, decay)
    return list(reachability)
