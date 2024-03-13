from enum import Enum
from typing import TypeAlias
import numpy as np

from . import _pyaccess_ext
from .graph import Graph
from .util import _build_graph, _build_ch_graph, _build_overlay_graph, _build_ch_graph_2, _build_transit_graph

class OneToManyType(Enum):
    RANGE_DIJKSTRA = 0
    RANGE_PHAST = 1
    RANGE_RPHAST = 3
    RANGE_RPHAST_GS = 4
    GRASP = 5

RANGE_DIJKSTRA = OneToManyType.RANGE_DIJKSTRA
RANGE_PHAST = OneToManyType.RANGE_PHAST
RANGE_RPHAST = OneToManyType.RANGE_RPHAST
RANGE_RPHAST_GS = OneToManyType.RANGE_RPHAST_GS
GRASP = OneToManyType.GRASP

OneToManySolver: TypeAlias = _pyaccess_ext.RangeDijkstra | _pyaccess_ext.RangePHAST | _pyaccess_ext.RangeRPHAST

def calc_2sfca(graph: Graph, dem_points: list[tuple[float, float]], dem_weight: list[int], sup_points: list[tuple[float, float]], sup_weight: list[int], decay: _pyaccess_ext.IDistanceDecay = _pyaccess_ext.BinaryDecay(900), algorithm: OneToManyType = RANGE_DIJKSTRA, weight: str = "default", partition: str | None = None, ch: str | None = None, overlay: str | None = None, transit: str | None = None, transit_weight: str | None = None, min_departure: int = 0, max_departure: int = 1000000) -> list[float]:
    match algorithm:
        case OneToManyType.RANGE_DIJKSTRA:
            if transit is not None and transit_weight is not None:
                tg = _build_transit_graph(graph, transit, transit_weight)
                solver = _pyaccess_ext.build_transit_dijkstra_solver(tg)
                solver.set_min_departure(min_departure)
                solver.set_max_departure(max_departure)
            else:
                g = _build_graph(graph, weight)
                if isinstance(g, _pyaccess_ext.Graph):
                    solver = _pyaccess_ext.build_range_dijkstra_solver(g)
                else:
                    solver = _pyaccess_ext.build_range_dijkstra_tc_solver(g)
        case OneToManyType.RANGE_PHAST:
            if ch is None:
                raise ValueError("no ch specified")
            g = _build_ch_graph(graph, ch)
            solver = _pyaccess_ext.build_range_phast_solver(g)
        case OneToManyType.RANGE_RPHAST:
            if ch is None:
                raise ValueError("no ch specified")
            g = _build_ch_graph(graph, ch)
            solver = _pyaccess_ext.build_range_rphast_solver(g)
        case OneToManyType.RANGE_RPHAST_GS:
            if ch is None:
                raise ValueError("no ch specified")
            g = _build_ch_graph_2(graph, ch)
            solver = _pyaccess_ext.build_range_rphast_gs_solver(g)
        case OneToManyType.GRASP:
            if overlay is None:
                raise ValueError("no overlay specified")
            g = _build_overlay_graph(graph, overlay)
            solver = _pyaccess_ext.build_grasp_solver(g)
        case _:
            raise NotImplementedError("")

    dem_lon = np.zeros((len(dem_points),), dtype=np.float32)
    dem_lat = np.zeros((len(dem_points),), dtype=np.float32)
    for i in range(len(dem_points)):
        dem_lon[i], dem_lat[i] = dem_points[i]
    sup_lon = np.zeros((len(sup_points),), dtype=np.float32)
    sup_lat = np.zeros((len(sup_points),), dtype=np.float32)
    for i in range(len(sup_points)):
        sup_lon[i], sup_lat[i] = sup_points[i]
    dem_nodes = graph._map_to_closest(dem_lon, dem_lat, ch=ch, overlay=overlay)
    sup_nodes = graph._map_to_closest(sup_lon, sup_lat, ch=ch, overlay=overlay)
    dem_weights = np.zeros((len(dem_points),), dtype=np.int32)
    for i in range(len(dem_points)):
        dem_weights[i] = dem_weight[i]
    sup_weights = np.zeros((len(sup_points),), dtype=np.int32)
    for i in range(len(sup_points)):
        sup_weights[i] = sup_weight[i]

    access = _pyaccess_ext.calc_2sfca(solver, dem_nodes, dem_weights, sup_nodes, sup_weights, decay)
    return access.tolist()

def calc_range(graph: Graph, sup_point: tuple[float, float], dem_points: list[tuple[float, float]], max_range: int = 900, algorithm: OneToManyType = RANGE_DIJKSTRA, weight: str = "default", ch: str | None = None, overlay: str | None = None, transit: str | None = None, transit_weight: str | None = None, min_departure: int = 0, max_departure: int = 1000000) -> list[int]:
    match algorithm:
        case OneToManyType.RANGE_DIJKSTRA:
            if transit is not None and transit_weight is not None:
                tg = _build_transit_graph(graph, transit, transit_weight)
                solver = _pyaccess_ext.build_transit_dijkstra_solver(tg)
                solver.set_min_departure(min_departure)
                solver.set_max_departure(max_departure)
            else:
                g = _build_graph(graph, weight)
                if isinstance(g, _pyaccess_ext.Graph):
                    solver = _pyaccess_ext.build_range_dijkstra_solver(g)
                else:
                    solver = _pyaccess_ext.build_range_dijkstra_tc_solver(g)
        case OneToManyType.RANGE_PHAST:
            if ch is None:
                raise ValueError("no ch specified")
            g = _build_ch_graph(graph, ch)
            solver = _pyaccess_ext.build_range_phast_solver(g)
        case OneToManyType.RANGE_RPHAST:
            if ch is None:
                raise ValueError("no ch specified")
            g = _build_ch_graph(graph, ch)
            solver = _pyaccess_ext.build_range_rphast_solver(g)
        case OneToManyType.RANGE_RPHAST_GS:
            if ch is None:
                raise ValueError("no ch specified")
            g = _build_ch_graph_2(graph, ch)
            solver = _pyaccess_ext.build_range_rphast_gs_solver(g)
        case OneToManyType.GRASP:
            if overlay is None:
                raise ValueError("no overlay specified")
            g = _build_overlay_graph(graph, overlay)
            solver = _pyaccess_ext.build_grasp_solver(g)
        case _:
            raise NotImplementedError("")

    dem_lon = np.zeros((len(dem_points),), dtype=np.float32)
    dem_lat = np.zeros((len(dem_points),), dtype=np.float32)
    for i in range(len(dem_points)):
        dem_lon[i], dem_lat[i] = dem_points[i]
    dem_nodes = graph._map_to_closest(dem_lon, dem_lat, ch=ch, overlay=overlay)
    sup_node = graph._get_closest(sup_point[0], sup_point[1], ch=ch, overlay=overlay)

    ranges = _pyaccess_ext.calc_range_query(solver, sup_node, dem_nodes, max_range)
    return ranges.tolist()

def calc_matrix(graph: Graph, sup_points: list[tuple[float, float]], dem_points: list[tuple[float, float]], max_range: int = 900, algorithm: OneToManyType = RANGE_DIJKSTRA, weight: str = "default", ch: str | None = None, overlay: str | None = None, transit: str | None = None, transit_weight: str | None = None, min_departure: int = 0, max_departure: int = 1000000) -> np.ndarray:
    match algorithm:
        case OneToManyType.RANGE_DIJKSTRA:
            if transit is not None and transit_weight is not None:
                tg =_build_transit_graph(graph, transit, transit_weight)
                solver = _pyaccess_ext.build_transit_dijkstra_solver(tg)
                solver.set_min_departure(min_departure)
                solver.set_max_departure(max_departure)
            else:
                g = _build_graph(graph, weight)
                if isinstance(g, _pyaccess_ext.Graph):
                    solver = _pyaccess_ext.build_range_dijkstra_solver(g)
                else:
                    solver = _pyaccess_ext.build_range_dijkstra_tc_solver(g)
        case OneToManyType.RANGE_PHAST:
            if ch is None:
                raise ValueError("no ch specified")
            g = _build_ch_graph(graph, ch)
            solver = _pyaccess_ext.build_range_phast_solver(g)
        case OneToManyType.RANGE_RPHAST:
            if ch is None:
                raise ValueError("no ch specified")
            g = _build_ch_graph(graph, ch)
            solver = _pyaccess_ext.build_range_rphast_solver(g)
        case OneToManyType.RANGE_RPHAST_GS:
            if ch is None:
                raise ValueError("no ch specified")
            g = _build_ch_graph_2(graph, ch)
            solver = _pyaccess_ext.build_range_rphast_gs_solver(g)
        case OneToManyType.GRASP:
            if overlay is None:
                raise ValueError("no overlay specified")
            g = _build_overlay_graph(graph, overlay)
            solver = _pyaccess_ext.build_grasp_solver(g)
        case _:
            raise NotImplementedError("")

    dem_lon = np.zeros((len(dem_points),), dtype=np.float32)
    dem_lat = np.zeros((len(dem_points),), dtype=np.float32)
    for i in range(len(dem_points)):
        dem_lon[i], dem_lat[i] = dem_points[i]
    sup_lon = np.zeros((len(sup_points),), dtype=np.float32)
    sup_lat = np.zeros((len(sup_points),), dtype=np.float32)
    for i in range(len(sup_points)):
        sup_lon[i], sup_lat[i] = sup_points[i]
    dem_nodes = graph._map_to_closest(dem_lon, dem_lat, ch=ch, overlay=overlay)
    sup_nodes = graph._map_to_closest(sup_lon, sup_lat, ch=ch, overlay=overlay)

    matrix = _pyaccess_ext.calc_matrix_query(solver, sup_nodes, dem_nodes, max_range)
    return matrix

def calc_reachability(graph: Graph, dem_points: list[tuple[float, float]], sup_points: list[tuple[float, float]], sup_weight: list[int], decay: _pyaccess_ext.IDistanceDecay = _pyaccess_ext.BinaryDecay(900), algorithm: OneToManyType = RANGE_DIJKSTRA, weight: str = "default", transit: str | None = None, transit_weight: str | None = None, min_departure: int = 0, max_departure: int = 1000000) -> list[float]:
    match algorithm:
        case OneToManyType.RANGE_DIJKSTRA:
            if transit is not None and transit_weight is not None:
                tg =_build_transit_graph(graph, transit, transit_weight)
                solver = _pyaccess_ext.build_transit_dijkstra_solver(tg)
                solver.set_min_departure(min_departure)
                solver.set_max_departure(max_departure)
            else:
                g = _build_graph(graph, weight)
                if isinstance(g, _pyaccess_ext.Graph):
                    solver = _pyaccess_ext.build_range_dijkstra_solver(g)
                else:
                    solver = _pyaccess_ext.build_range_dijkstra_tc_solver(g)
        case _:
            raise NotImplementedError("")

    dem_lon = np.zeros((len(dem_points),), dtype=np.float32)
    dem_lat = np.zeros((len(dem_points),), dtype=np.float32)
    for i in range(len(dem_points)):
        dem_lon[i], dem_lat[i] = dem_points[i]
    sup_lon = np.zeros((len(sup_points),), dtype=np.float32)
    sup_lat = np.zeros((len(sup_points),), dtype=np.float32)
    for i in range(len(sup_points)):
        sup_lon[i], sup_lat[i] = sup_points[i]
    dem_nodes = graph._map_to_closest(dem_lon, dem_lat)
    sup_nodes = graph._map_to_closest(sup_lon, sup_lat)
    sup_weights = np.zeros((len(sup_points),), dtype=np.int32)
    for i in range(len(sup_points)):
        sup_weights[i] = sup_weight[i]

    reachability = _pyaccess_ext.calc_reachability(solver, dem_nodes, sup_nodes, sup_weights, decay)
    return reachability.tolist()
