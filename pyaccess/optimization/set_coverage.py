from scipy.optimize import milp, Bounds, LinearConstraint
import numpy as np
import geopandas as gpd
import pandas as pd

from ..util import transform_locations, transform_weights
from ..accessibility import calc_matrix
from ..graph import Graph

def set_coverage(
        graph: Graph,
        demand_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        supply_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        percent_covered: float,
        max_range: int = 1800
    ) -> np.ndarray:
    # compute td-matrix
    dem_lon, dem_lat = transform_locations(demand_locations)
    sup_lon, sup_lat = transform_locations(supply_locations)
    matrix = calc_matrix(graph, (sup_lon, sup_lat), (dem_lon, dem_lat), max_range)
    # run optimization
    n_x = sup_lon.shape[0]
    n_y = dem_lon.shape[0]
    solution = _set_coverage(matrix, np.ones((n_y,)), np.ones((n_x,)), int(n_y * percent_covered))
    return solution

def _set_coverage(td_matrix: np.ndarray, demand_weights: np.ndarray, supply_costs: np.ndarray, min_covered_weight: int) -> np.ndarray:
    """Computes a subset of the supply_locations minimizing the cost while also covering a minimum amount of demand.

    Args:
        td_matrix: [n x m] Travel distance matrix
        demand_weights: [m x 1] List of demand weights
        supply_costs: [n x 1] List of supply costs (might be negative)
        min_covered_weight: Minimum amount of covered demand

    Returns:
        [n x 1] List of selected locations (1 = selected, 0 = not selected)
    """
    n = td_matrix.shape[0]
    m = td_matrix.shape[1]
    # minimizing supply costs
    c = np.zeros((n+m,))
    c[:n] = supply_costs
    # get size of constraint matrix
    size = 1 + m
    # build contraint matrix
    A = np.zeros((size, n+m))
    ub = np.ones((size,))
    lb = np.ones((size,))
    # set coverage contraint
    A[0,n:] = demand_weights
    ub[0] = np.sum(demand_weights)
    lb[0] = min_covered_weight
    # set coverage constraint for every demand location
    for i in range(0, m):
        for j in range(0, n):
            dist = td_matrix[j, i]
            if dist >= 0:
                A[i+1, j] = 1
        A[1+i, n+i] = -1
        lb[1+i] = 0
        ub[1+i] = n - 1
    # run optimization
    bounds = Bounds(0, 1)
    integrality = np.full((n+m,), 1)
    constraints = LinearConstraint(A=A, lb=lb, ub=ub)
    res = milp(c, bounds=bounds, integrality=integrality, constraints=constraints)
    # extract solution
    if res.x is None:
        raise ValueError("Optimization failed.")
    result: np.ndarray = res.x[:n]
    return result.astype("bool")
