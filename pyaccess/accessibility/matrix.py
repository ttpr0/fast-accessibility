import numpy as np
from shapely import Point
import geopandas as gpd

from .. import _pyaccess_ext
from ..graph import Graph
from ..util import _build_graph, _build_ch_graph, _build_overlay_graph, _build_ch_graph_2, _build_transit_graph
from ..util import transform_location, transform_locations, transform_weights
from .algorithms import OneToManyType, RANGE_DIJKSTRA, RANGE_PHAST, RANGE_RPHAST, RANGE_RPHAST_GS, GRASP


#*********************************************************
# one-to-many range
#*********************************************************

def calc_range(
        graph: Graph,
        sup_point: tuple[float, float] | np.ndarray | Point,
        dem_points: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        max_range: int = 900,
        algorithm: OneToManyType = RANGE_DIJKSTRA,
        weight: str = "default",
        ch: str | None = None,
        overlay: str | None = None,
        transit: str | None = None,
        transit_weight: str | None = None,
        min_departure: int = 0,
        max_departure: int = 1000000
    ) -> np.ndarray:
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

    dem_lon, dem_lat = transform_locations(dem_points)
    dem_nodes = graph._map_to_closest(dem_lon, dem_lat, ch=ch, overlay=overlay)
    sup_lon, sup_lat = transform_location(sup_point)
    sup_node = graph._get_closest(sup_lon, sup_lat, ch=ch, overlay=overlay)

    ranges = _pyaccess_ext.calc_range_query(solver, sup_node, dem_nodes, max_range)
    return ranges

#*********************************************************
# many-to-many range
#*********************************************************

def calc_matrix(
        graph: Graph,
        sup_points: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        dem_points: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        max_range: int = 900,
        algorithm: OneToManyType = RANGE_DIJKSTRA,
        weight: str = "default",
        ch: str | None = None,
        overlay: str | None = None,
        transit: str | None = None,
        transit_weight: str | None = None,
        min_departure: int = 0,
        max_departure: int = 1000000
    ) -> np.ndarray:
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

    dem_lon, dem_lat = transform_locations(dem_points)
    sup_lon, sup_lat = transform_locations(sup_points)
    dem_nodes = graph._map_to_closest(dem_lon, dem_lat, ch=ch, overlay=overlay)
    sup_nodes = graph._map_to_closest(sup_lon, sup_lat, ch=ch, overlay=overlay)

    matrix = _pyaccess_ext.calc_matrix_query(solver, sup_nodes, dem_nodes, max_range)
    return matrix
