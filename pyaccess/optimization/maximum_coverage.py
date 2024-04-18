from scipy.optimize import milp, Bounds, LinearConstraint
import numpy as np
import geopandas as gpd
import pandas as pd

from ..util import transform_locations, transform_weights
from ..accessibility import calc_matrix
from ..graph import Graph


def maximum_coverage(
        graph: Graph,
        demand_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        supply_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        k: int,
        max_range: int = 1800
    ) -> np.ndarray:
    # compute td-matrix
    dem_lon, dem_lat = transform_locations(demand_locations)
    sup_lon, sup_lat = transform_locations(supply_locations)
    matrix = calc_matrix(graph, (sup_lon, sup_lat), (dem_lon, dem_lat), max_range)
    # run optimization
    n_x = sup_lon.shape[0]
    n_y = dem_lon.shape[0]
    solution = _maximum_coverage(matrix, np.ones((n_y,)), np.ones((n_x,)), k)
    return solution

def weighted_maximum_coverage(
        graph: Graph,
        demand_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        demand_weights: list[int] | np.ndarray | pd.Series,
        supply_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        k: int,
        max_range: int = 1800
    ) -> np.ndarray:
    # compute td-matrix
    dem_lon, dem_lat = transform_locations(demand_locations)
    sup_lon, sup_lat = transform_locations(supply_locations)
    matrix = calc_matrix(graph, (sup_lon, sup_lat), (dem_lon, dem_lat), max_range)
    # run optimization
    n_x = sup_lon.shape[0]
    n_y = dem_lon.shape[0]
    dem_weights = transform_weights(demand_weights)
    solution = _maximum_coverage(matrix, dem_weights, np.ones((n_x,)), k)
    return solution

def budgeted_maximum_coverage(
        graph: Graph,
        demand_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        demand_weights: list[int] | np.ndarray | pd.Series,
        supply_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        supply_costs: list[int] | np.ndarray | pd.Series,
        is_removal_cost: list[bool] | np.ndarray | pd.Series,
        max_cost: int,
        max_range: int = 1800
    ) -> np.ndarray:
    # compute td-matrix
    dem_lon, dem_lat = transform_locations(demand_locations)
    sup_lon, sup_lat = transform_locations(supply_locations)
    matrix = calc_matrix(graph, (sup_lon, sup_lat), (dem_lon, dem_lat), max_range)
    # run optimization
    n_x = sup_lon.shape[0]
    n_y = dem_lon.shape[0]
    dem_weights = transform_weights(demand_weights)
    _max_cost = max_cost
    _min_cost = 0
    _costs = np.zeros((n_y,))
    for i in range(n_y):
        if is_removal_cost[i]:
            _costs[i] = -supply_costs[i]
            _max_cost -= supply_costs[i]
            _min_cost -= supply_costs[i]
        else:
            _costs[i] = supply_costs[i]
    solution = _maximum_coverage(matrix, dem_weights, _costs, _max_cost, _min_cost)
    return solution

def _maximum_coverage(td_matrix: np.ndarray, demand_weights: np.ndarray, supply_costs: np.ndarray, max_cost: int, min_cost: int = 0) -> np.ndarray:
    """Computes a subset of the supply_locations maximizing the covered demand.

    Args:
        td_matrix: [n x m] Travel distance matrix
        demand_weights: [m x 1] List of demand weights
        supply_costs: [n x 1] List of supply costs (might be negative)
        max_cost: Maximum cost
        min_cost: Minimum (possibile) cost (used as lower bound in MILP)

    Returns:
        [n x 1] List of selected locations (1 = selected, 0 = not selected)
    """
    n_x = td_matrix.shape[0]
    n_y = td_matrix.shape[1]
    # maximizing amount of covered demand
    c = np.zeros((n_y+n_x,))
    c[:n_y] = -demand_weights
    # get size of constraint matrix
    size = 1 + n_y
    # build contraint matrix
    A = np.zeros((size, n_y+n_x))
    ub = np.ones((size,))
    lb = np.ones((size,))
    # no more than max-cost for supply_locations
    A[0,n_y:] = supply_costs
    ub[0] = max_cost
    lb[0] = min_cost
    # set coverage constraint for every demand location
    for i in range(0, n_y):
        for j in range(0, n_x):
            dist = td_matrix[j, i]
            if dist >= 0:
                A[i+1, n_y+j] = 1
        A[i+1, i] = -1
        lb[i+1] = 0
        ub[i+1] = n_x - 1
    # run optimization
    bounds = Bounds(0, 1)
    integrality = np.full((n_y+n_x,), 1)
    constraints = LinearConstraint(A=A, lb=lb, ub=ub)
    res = milp(c, bounds=bounds, integrality=integrality, constraints=constraints)
    # extract solution
    if res.x is None:
        raise ValueError("Optimization failed.")
    result: np.ndarray = res.x[n_y:]
    return result.astype("bool")
