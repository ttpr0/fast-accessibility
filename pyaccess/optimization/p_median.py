from scipy.optimize import milp, Bounds, LinearConstraint
import numpy as np
import geopandas as gpd
import pandas as pd

from ..util import transform_locations, transform_weights
from ..accessibility import calc_matrix
from ..graph import Graph

def p_median(
        graph: Graph,
        demand_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        demand_weights: list[int] | np.ndarray | pd.Series,
        supply_locations: list[tuple[float, float]] | tuple[np.ndarray, np.ndarray] | gpd.GeoSeries,
        supply_capacity: list[int] | np.ndarray | pd.Series,
        k: int,
        max_range: int = 1800
    ) -> np.ndarray:
    # compute td-matrix
    matrix = calc_matrix(graph, supply_locations, demand_locations, max_range)
    # run optimization
    dem_weight = transform_weights(demand_weights)
    sup_capacity = transform_weights(supply_capacity)
    solution = _p_median(matrix, dem_weight, sup_capacity, np.ones(sup_capacity.shape), k)
    return solution

def _p_median(td_matrix: np.ndarray, demand_weights: np.ndarray, supply_capacity: np.ndarray, supply_costs: np.ndarray, max_cost: int, min_cost: int = 0) -> np.ndarray:
    """Computes a subset of the supply_locations minimizing the distance between supply and demand.

    Args:
        td_matrix: [n x m] Travel distance matrix
        demand_weights: [m x 1] List of demand weights
        supply_capacity: [n x 1] List of supply capacities
        supply_costs: [n x 1] List of supply costs (might be negative)
        max_cost: Maximum cost
        min_cost: Minimum (possibile) cost (used as lower bound in MILP)

    Returns:
        [n x 1] List of selected locations (1 = selected, 0 = not selected)
    """
    n = td_matrix.shape[0]
    m = td_matrix.shape[1]
    # get size of constraint matrix
    count = n + n*m
    size = 1 + n + m
    # minimizing distances between supply and demand
    c = np.zeros((count,))
    _c = td_matrix.copy()
    _d = np.broadcast_to(demand_weights, (n, m))
    _c = _c * _d
    c[n:] = _c.reshape((-1,))
    # build contraint matrix
    A = np.zeros((size, count))
    ub = np.ones((size,))
    lb = np.ones((size,))
    # no more than max-cost for supply_locations
    A[0,:n] = supply_costs
    ub[0] = max_cost
    lb[0] = min_cost
    # supply capacity <= allocated demand
    for i in range(0, n):
        A[1+i, i] = -supply_capacity[i]
        s = n + i*m
        e = n + (i+1)*m
        A[1+i, s:e] = demand_weights
        ub[1+i] = 0
        lb[1+i] = -supply_capacity[i]
    # all demand covered by supply locations
    for i in range(0, m):
        A[1+n+i, n+i::n] = 1
        lb[1+n+i] = 1
        ub[1+n+i] = 1
    # run optimization
    bounds = Bounds(0, 1)
    integrality = np.zeros((count,))
    integrality[:n] = 1
    constraints = LinearConstraint(A=A, lb=lb, ub=ub)
    res = milp(c, bounds=bounds, integrality=integrality, constraints=constraints)
    # extract solution
    if res.x is None:
        raise ValueError("Optimization failed.")
    result: np.ndarray = res.x[:n]
    return result.astype("bool")
