from typing import Any, Optional, overload, Sequence, Iterable, Iterator as iterator
from enum import Enum
import numpy.typing
import collections.abc
import pyaccess._pyaccess_ext

BACKWARD: Direction

class BinaryDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, max_dist: int) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class CHData:
    """
    None
    """

    def get_adjacent_shortcuts(self, arg0: int, arg1: pyaccess._pyaccess_ext.Direction, /) -> list:
        ...

    def get_node_level(self, arg: int, /) -> int:
        ...

    def get_shortcut(self, arg: int, /) -> pyaccess._pyaccess_ext.Shortcut:
        ...

    def shortcut_count(self) -> int:
        ...

class CHGraph(pyaccess._pyaccess_ext.ICHGraph, pyaccess._pyaccess_ext.IGraph):
    """
    None
    """
    ...

class CHGraph2(pyaccess._pyaccess_ext.IGraph):
    """
    None
    """
    ...

class CHIndex:
    """
    None
    """
    ...

class CellIndex:
    """
    None
    """
    ...

class Connection:
    """
    None
    """

    @overload
    def __init__(self, arg0: int, arg1: int, arg2: int, /) -> None:
        """
        __init__(self, arg0: int, arg1: int, arg2: int, /) -> None
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        __init__(self) -> None
        """
        ...

    @property
    def route(self) -> int:
        ...
    @route.setter
    def route(self, arg: int, /) -> None:
        ...

    @property
    def stop_a(self) -> int:
        ...
    @stop_a.setter
    def stop_a(self, arg: int, /) -> None:
        ...

    @property
    def stop_b(self) -> int:
        ...
    @stop_b.setter
    def stop_b(self, arg: int, /) -> None:
        ...

class ConnectionVector:
    """
    None
    """

    def __bool__(self) -> bool:
        """
        Check whether the vector is nonempty
        """
        ...

    @overload
    def __delitem__(self, arg: slice, /) -> None:
        """
        __delitem__(self, arg: slice, /) -> None
        """
        ...

    @overload
    def __delitem__(self, arg: int, /) -> None:
        """
        __delitem__(self, arg: int, /) -> None
        """
        ...

    @overload
    def __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.ConnectionVector:
        """
        __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.ConnectionVector
        """
        ...

    @overload
    def __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.Connection:
        """
        __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.Connection
        """
        ...

    @overload
    def __init__(self, arg: collections.abc.Iterable[pyaccess._pyaccess_ext.Connection], /) -> None:
        """
        Construct from an iterable object
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        Default constructor
        """
        ...

    @overload
    def __init__(self, arg: pyaccess._pyaccess_ext.ConnectionVector) -> None:
        """
        Copy constructor
        """
        ...

    def __iter__(self) -> collections.abc.Iterator[pyaccess._pyaccess_ext.Connection]:
        ...

    def __len__(self) -> int:
        ...

    def __repr__(self) -> str:
        ...

    @overload
    def __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.ConnectionVector, /) -> None:
        """
        __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.ConnectionVector, /) -> None
        """
        ...

    @overload
    def __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.Connection, /) -> None:
        """
        __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.Connection, /) -> None
        """
        ...

    def append(self, arg: pyaccess._pyaccess_ext.Connection, /) -> None:
        """
        Append `arg` to the end of the list.
        """
        ...

    def clear(self) -> None:
        """
        Remove all items from list.
        """
        ...

    def extend(self, arg: pyaccess._pyaccess_ext.ConnectionVector, /) -> None:
        """
        Extend `self` by appending elements from `arg`.
        """
        ...

    def insert(self, arg0: int, arg1: pyaccess._pyaccess_ext.Connection, /) -> None:
        """
        Insert object `arg1` before index `arg0`.
        """
        ...

    def pop(self, index: int = -1) -> pyaccess._pyaccess_ext.Connection:
        """
        Remove and return item at `index` (default last).
        """
        ...

class Coord:
    """
    None
    """

    @overload
    def __init__(self, arg0: float, arg1: float, /) -> None:
        """
        __init__(self, arg0: float, arg1: float, /) -> None
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        __init__(self) -> None
        """
        ...

    @property
    def lat(self) -> float:
        ...
    @lat.setter
    def lat(self, arg: float, /) -> None:
        ...

    @property
    def lon(self) -> float:
        ...
    @lon.setter
    def lon(self, arg: float, /) -> None:
        ...

class CoordVector:
    """
    None
    """

    def __bool__(self) -> bool:
        """
        Check whether the vector is nonempty
        """
        ...

    @overload
    def __delitem__(self, arg: slice, /) -> None:
        """
        __delitem__(self, arg: slice, /) -> None
        """
        ...

    @overload
    def __delitem__(self, arg: int, /) -> None:
        """
        __delitem__(self, arg: int, /) -> None
        """
        ...

    @overload
    def __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.CoordVector:
        """
        __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.CoordVector
        """
        ...

    @overload
    def __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.Coord:
        """
        __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.Coord
        """
        ...

    @overload
    def __init__(self, arg: collections.abc.Iterable[pyaccess._pyaccess_ext.Coord], /) -> None:
        """
        Construct from an iterable object
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        Default constructor
        """
        ...

    @overload
    def __init__(self, arg: pyaccess._pyaccess_ext.CoordVector) -> None:
        """
        Copy constructor
        """
        ...

    def __iter__(self) -> collections.abc.Iterator[pyaccess._pyaccess_ext.Coord]:
        ...

    def __len__(self) -> int:
        ...

    def __repr__(self) -> str:
        ...

    @overload
    def __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.CoordVector, /) -> None:
        """
        __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.CoordVector, /) -> None
        """
        ...

    @overload
    def __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.Coord, /) -> None:
        """
        __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.Coord, /) -> None
        """
        ...

    @overload
    def append(self, arg: tuple[float, float], /) -> None:
        """
        Insert Coordinate with (lon, lat).
        """
        ...

    @overload
    def append(self, arg: pyaccess._pyaccess_ext.Coord, /) -> None:
        """
        Append `arg` to the end of the list.
        """
        ...

    def clear(self) -> None:
        """
        Remove all items from list.
        """
        ...

    def extend(self, arg: pyaccess._pyaccess_ext.CoordVector, /) -> None:
        """
        Extend `self` by appending elements from `arg`.
        """
        ...

    def insert(self, arg0: int, arg1: pyaccess._pyaccess_ext.Coord, /) -> None:
        """
        Insert object `arg1` before index `arg0`.
        """
        ...

    def pop(self, index: int = -1) -> pyaccess._pyaccess_ext.Coord:
        """
        Remove and return item at `index` (default last).
        """
        ...

class DSnap:
    """
    None
    """

    def __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.Snap:
        ...

    @overload
    def __init__(self, arg0: pyaccess._pyaccess_ext.Snap, arg1: pyaccess._pyaccess_ext.Snap, /) -> None:
        """
        __init__(self, arg0: pyaccess._pyaccess_ext.Snap, arg1: pyaccess._pyaccess_ext.Snap, /) -> None
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        __init__(self) -> None
        """
        ...

    @overload
    def __init__(self, arg: int, /) -> None:
        """
        __init__(self, arg: int, /) -> None
        """
        ...

    @overload
    def __init__(self, arg: pyaccess._pyaccess_ext.Snap, /) -> None:
        """
        __init__(self, arg: pyaccess._pyaccess_ext.Snap, /) -> None
        """
        ...

    def __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.Snap, /) -> None:
        ...

    def len(self) -> int:
        ...

class Direction(Enum):
    """
    Incoming or outgoing edges.
    """

    BACKWARD: Direction

    FORWARD: Direction

class Edge:
    """
    None
    """

    @overload
    def __init__(self, arg0: int, arg1: int, /) -> None:
        """
        __init__(self, arg0: int, arg1: int, /) -> None
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        __init__(self) -> None
        """
        ...

    @property
    def node_a(self) -> int:
        ...
    @node_a.setter
    def node_a(self, arg: int, /) -> None:
        ...

    @property
    def node_b(self) -> int:
        ...
    @node_b.setter
    def node_b(self, arg: int, /) -> None:
        ...

class EdgeVector:
    """
    None
    """

    def __bool__(self) -> bool:
        """
        Check whether the vector is nonempty
        """
        ...

    @overload
    def __delitem__(self, arg: slice, /) -> None:
        """
        __delitem__(self, arg: slice, /) -> None
        """
        ...

    @overload
    def __delitem__(self, arg: int, /) -> None:
        """
        __delitem__(self, arg: int, /) -> None
        """
        ...

    @overload
    def __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.EdgeVector:
        """
        __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.EdgeVector
        """
        ...

    @overload
    def __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.Edge:
        """
        __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.Edge
        """
        ...

    @overload
    def __init__(self, arg: collections.abc.Iterable[pyaccess._pyaccess_ext.Edge], /) -> None:
        """
        Construct from an iterable object
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        Default constructor
        """
        ...

    @overload
    def __init__(self, arg: pyaccess._pyaccess_ext.EdgeVector) -> None:
        """
        Copy constructor
        """
        ...

    def __iter__(self) -> collections.abc.Iterator[pyaccess._pyaccess_ext.Edge]:
        ...

    def __len__(self) -> int:
        ...

    def __repr__(self) -> str:
        ...

    @overload
    def __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.EdgeVector, /) -> None:
        """
        __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.EdgeVector, /) -> None
        """
        ...

    @overload
    def __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.Edge, /) -> None:
        """
        __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.Edge, /) -> None
        """
        ...

    def append(self, arg: pyaccess._pyaccess_ext.Edge, /) -> None:
        """
        Append `arg` to the end of the list.
        """
        ...

    def clear(self) -> None:
        """
        Remove all items from list.
        """
        ...

    def extend(self, arg: pyaccess._pyaccess_ext.EdgeVector, /) -> None:
        """
        Extend `self` by appending elements from `arg`.
        """
        ...

    def from_array(self, arg0: numpy.typing.NDArray, arg1: numpy.typing.NDArray, /) -> None:
        ...

    def insert(self, arg0: int, arg1: pyaccess._pyaccess_ext.Edge, /) -> None:
        """
        Insert object `arg1` before index `arg0`.
        """
        ...

    def pop(self, index: int = -1) -> pyaccess._pyaccess_ext.Edge:
        """
        Remove and return item at `index` (default last).
        """
        ...

class ExponentialDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, arg: int, /) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

FORWARD: Direction

class FloatVector:
    """
    None
    """

    def __bool__(self) -> bool:
        """
        Check whether the vector is nonempty
        """
        ...

    @overload
    def __contains__(self, arg: object, /) -> bool:
        """
        __contains__(self, arg: object, /) -> bool
        """
        ...

    @overload
    def __contains__(self, arg: float, /) -> bool:
        """
        __contains__(self, arg: float, /) -> bool
        """
        ...

    @overload
    def __delitem__(self, arg: slice, /) -> None:
        """
        __delitem__(self, arg: slice, /) -> None
        """
        ...

    @overload
    def __delitem__(self, arg: int, /) -> None:
        """
        __delitem__(self, arg: int, /) -> None
        """
        ...

    @overload
    def __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.FloatVector:
        """
        __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.FloatVector
        """
        ...

    @overload
    def __getitem__(self, arg: int, /) -> float:
        """
        __getitem__(self, arg: int, /) -> float
        """
        ...

    @overload
    def __init__(self, arg: collections.abc.Iterable[float], /) -> None:
        """
        Construct from an iterable object
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        Default constructor
        """
        ...

    @overload
    def __init__(self, arg: pyaccess._pyaccess_ext.FloatVector) -> None:
        """
        Copy constructor
        """
        ...

    def __iter__(self) -> collections.abc.Iterator[float]:
        ...

    def __len__(self) -> int:
        ...

    def __repr__(self) -> str:
        ...

    @overload
    def __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.FloatVector, /) -> None:
        """
        __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.FloatVector, /) -> None
        """
        ...

    @overload
    def __setitem__(self, arg0: int, arg1: float, /) -> None:
        """
        __setitem__(self, arg0: int, arg1: float, /) -> None
        """
        ...

    def append(self, arg: float, /) -> None:
        """
        Append `arg` to the end of the list.
        """
        ...

    def clear(self) -> None:
        """
        Remove all items from list.
        """
        ...

    def count(self, arg: float, /) -> int:
        """
        Return number of occurrences of `arg`.
        """
        ...

    def extend(self, arg: pyaccess._pyaccess_ext.FloatVector, /) -> None:
        """
        Extend `self` by appending elements from `arg`.
        """
        ...

    def insert(self, arg0: int, arg1: float, /) -> None:
        """
        Insert object `arg1` before index `arg0`.
        """
        ...

    def pop(self, index: int = -1) -> float:
        """
        Remove and return item at `index` (default last).
        """
        ...

    def remove(self, arg: float, /) -> None:
        """
        Remove first occurrence of `arg`.
        """
        ...

class GRASP:
    """
    None
    """

    def is_build(self) -> bool:
        ...

class GaussianDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, arg: int, /) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class Graph(pyaccess._pyaccess_ext.IGraph):
    """
    None
    """
    ...

class GraphBase:
    """
    None
    """

    def edge_count(self) -> int:
        ...

    def get_adjacent_edges(self, arg0: int, arg1: pyaccess._pyaccess_ext.Direction, /) -> list:
        ...

    def get_edge(self, arg: int, /) -> pyaccess._pyaccess_ext.Edge:
        ...

    def get_node(self, arg: int, /) -> pyaccess._pyaccess_ext.Node:
        ...

    def get_node_geom(self, arg: int, /) -> pyaccess._pyaccess_ext.Coord:
        ...

    def node_count(self) -> int:
        ...

class GravityDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, arg0: int, arg1: float, /) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class HybridDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, distances: pyaccess._pyaccess_ext.IntVector, factors: pyaccess._pyaccess_ext.FloatVector) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class ICHGraph(pyaccess._pyaccess_ext.IGraph):
    """
    None
    """
    ...

class IDMapping:
    """
    None
    """

    def get_source(self, arg: int, /) -> int:
        ...

    def get_target(self, arg: int, /) -> int:
        ...

class IDistanceDecay:
    """
    None
    """

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class IGraph:
    """
    None
    """
    ...

class IGraphIndex:
    """
    None
    """

    @overload
    def get_closest_node(self, arg0: float, arg1: float, arg2: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.DSnap:
        """
        get_closest_node(self, arg0: float, arg1: float, arg2: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.DSnap
        """
        ...

    @overload
    def get_closest_node(self, arg: pyaccess._pyaccess_ext.Coord, /) -> pyaccess._pyaccess_ext.DSnap:
        """
        get_closest_node(self, arg: pyaccess._pyaccess_ext.Coord, /) -> pyaccess._pyaccess_ext.DSnap
        """
        ...

    @overload
    def get_closest_node(self, arg0: float, arg1: float, /) -> pyaccess._pyaccess_ext.DSnap:
        """
        get_closest_node(self, arg0: float, arg1: float, /) -> pyaccess._pyaccess_ext.DSnap
        """
        ...

    @overload
    def get_closest_node(self, arg0: pyaccess._pyaccess_ext.Coord, arg1: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.DSnap:
        """
        get_closest_node(self, arg0: pyaccess._pyaccess_ext.Coord, arg1: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.DSnap
        """
        ...

    @overload
    def map_to_closest(self, arg: pyaccess._pyaccess_ext.NodeVector, /) -> pyaccess._pyaccess_ext.SnapVector:
        """
        map_to_closest(self, arg: pyaccess._pyaccess_ext.NodeVector, /) -> pyaccess._pyaccess_ext.SnapVector
        """
        ...

    @overload
    def map_to_closest(self, arg: pyaccess._pyaccess_ext.CoordVector, /) -> pyaccess._pyaccess_ext.SnapVector:
        """
        map_to_closest(self, arg: pyaccess._pyaccess_ext.CoordVector, /) -> pyaccess._pyaccess_ext.SnapVector
        """
        ...

    @overload
    def map_to_closest(self, arg0: numpy.typing.NDArray, arg1: numpy.typing.NDArray, /) -> pyaccess._pyaccess_ext.SnapVector:
        """
        map_to_closest(self, arg0: numpy.ndarray[dtype=float32, shape=(*)], arg1: numpy.ndarray[dtype=float32, shape=(*)], /) -> pyaccess._pyaccess_ext.SnapVector
        """
        ...

    @overload
    def map_to_closest(self, arg0: pyaccess._pyaccess_ext.CoordVector, arg1: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.SnapVector:
        """
        map_to_closest(self, arg0: pyaccess._pyaccess_ext.CoordVector, arg1: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.SnapVector
        """
        ...

    @overload
    def map_to_closest(self, arg0: numpy.typing.NDArray, arg1: numpy.typing.NDArray, arg2: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.SnapVector:
        """
        map_to_closest(self, arg0: numpy.ndarray[dtype=float32, shape=(*)], arg1: numpy.ndarray[dtype=float32, shape=(*)], arg2: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.SnapVector
        """
        ...

class ITiledGraph(pyaccess._pyaccess_ext.IGraph):
    """
    None
    """
    ...

class IntVector:
    """
    None
    """

    def __bool__(self) -> bool:
        """
        Check whether the vector is nonempty
        """
        ...

    @overload
    def __contains__(self, arg: object, /) -> bool:
        """
        __contains__(self, arg: object, /) -> bool
        """
        ...

    @overload
    def __contains__(self, arg: int, /) -> bool:
        """
        __contains__(self, arg: int, /) -> bool
        """
        ...

    @overload
    def __delitem__(self, arg: slice, /) -> None:
        """
        __delitem__(self, arg: slice, /) -> None
        """
        ...

    @overload
    def __delitem__(self, arg: int, /) -> None:
        """
        __delitem__(self, arg: int, /) -> None
        """
        ...

    @overload
    def __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.IntVector:
        """
        __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.IntVector
        """
        ...

    @overload
    def __getitem__(self, arg: int, /) -> int:
        """
        __getitem__(self, arg: int, /) -> int
        """
        ...

    @overload
    def __init__(self, arg: collections.abc.Iterable[int], /) -> None:
        """
        Construct from an iterable object
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        Default constructor
        """
        ...

    @overload
    def __init__(self, arg: pyaccess._pyaccess_ext.IntVector) -> None:
        """
        Copy constructor
        """
        ...

    def __iter__(self) -> collections.abc.Iterator[int]:
        ...

    def __len__(self) -> int:
        ...

    def __repr__(self) -> str:
        ...

    @overload
    def __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.IntVector, /) -> None:
        """
        __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.IntVector, /) -> None
        """
        ...

    @overload
    def __setitem__(self, arg0: int, arg1: int, /) -> None:
        """
        __setitem__(self, arg0: int, arg1: int, /) -> None
        """
        ...

    def append(self, arg: int, /) -> None:
        """
        Append `arg` to the end of the list.
        """
        ...

    def clear(self) -> None:
        """
        Remove all items from list.
        """
        ...

    def count(self, arg: int, /) -> int:
        """
        Return number of occurrences of `arg`.
        """
        ...

    def extend(self, arg: pyaccess._pyaccess_ext.IntVector, /) -> None:
        """
        Extend `self` by appending elements from `arg`.
        """
        ...

    def insert(self, arg0: int, arg1: int, /) -> None:
        """
        Insert object `arg1` before index `arg0`.
        """
        ...

    def pop(self, index: int = -1) -> int:
        """
        Remove and return item at `index` (default last).
        """
        ...

    def remove(self, arg: int, /) -> None:
        """
        Remove first occurrence of `arg`.
        """
        ...

class InversePowerDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, arg: int, /) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class KernelDensityDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, arg0: int, arg1: float, /) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class LinearDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, max_dist: int) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class Node:
    """
    None
    """

    @overload
    def __init__(self, arg: pyaccess._pyaccess_ext.Coord, /) -> None:
        """
        __init__(self, arg: pyaccess._pyaccess_ext.Coord, /) -> None
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        __init__(self) -> None
        """
        ...

    @property
    def loc(self) -> pyaccess._pyaccess_ext.Coord:
        ...
    @loc.setter
    def loc(self, arg: pyaccess._pyaccess_ext.Coord, /) -> None:
        ...

class NodeVector:
    """
    None
    """

    def __bool__(self) -> bool:
        """
        Check whether the vector is nonempty
        """
        ...

    @overload
    def __delitem__(self, arg: slice, /) -> None:
        """
        __delitem__(self, arg: slice, /) -> None
        """
        ...

    @overload
    def __delitem__(self, arg: int, /) -> None:
        """
        __delitem__(self, arg: int, /) -> None
        """
        ...

    @overload
    def __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.NodeVector:
        """
        __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.NodeVector
        """
        ...

    @overload
    def __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.Node:
        """
        __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.Node
        """
        ...

    @overload
    def __init__(self, arg: collections.abc.Iterable[pyaccess._pyaccess_ext.Node], /) -> None:
        """
        Construct from an iterable object
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        Default constructor
        """
        ...

    @overload
    def __init__(self, arg: pyaccess._pyaccess_ext.NodeVector) -> None:
        """
        Copy constructor
        """
        ...

    def __iter__(self) -> collections.abc.Iterator[pyaccess._pyaccess_ext.Node]:
        ...

    def __len__(self) -> int:
        ...

    def __repr__(self) -> str:
        ...

    @overload
    def __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.NodeVector, /) -> None:
        """
        __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.NodeVector, /) -> None
        """
        ...

    @overload
    def __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.Node, /) -> None:
        """
        __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.Node, /) -> None
        """
        ...

    def append(self, arg: pyaccess._pyaccess_ext.Node, /) -> None:
        """
        Append `arg` to the end of the list.
        """
        ...

    def clear(self) -> None:
        """
        Remove all items from list.
        """
        ...

    def extend(self, arg: pyaccess._pyaccess_ext.NodeVector, /) -> None:
        """
        Extend `self` by appending elements from `arg`.
        """
        ...

    def from_array(self, arg0: numpy.typing.NDArray, arg1: numpy.typing.NDArray, /) -> None:
        ...

    def insert(self, arg0: int, arg1: pyaccess._pyaccess_ext.Node, /) -> None:
        """
        Insert object `arg1` before index `arg0`.
        """
        ...

    def pop(self, index: int = -1) -> pyaccess._pyaccess_ext.Node:
        """
        Remove and return item at `index` (default last).
        """
        ...

class Partition:
    """
    None
    """

    def get_node_tile(self, arg: int, /) -> int:
        ...

class PiecewiseLinearDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, arg0: pyaccess._pyaccess_ext.IntVector, arg1: pyaccess._pyaccess_ext.FloatVector, /) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class PolynomDecay(pyaccess._pyaccess_ext.IDistanceDecay):
    """
    None
    """

    def __init__(self, arg0: int, arg1: pyaccess._pyaccess_ext.FloatVector, /) -> None:
        ...

    def get_distance_weight(self, distance: int) -> float:
        """
        Computes the decayed weight (range [0, 1]) for the given distance.
        """
        ...

    def get_max_distance(self) -> int:
        """
        Maximum distance to give distance-weight greater than 0.
        """
        ...

class RangeDijkstra:
    """
    None
    """

    def is_build(self) -> bool:
        ...

class RangeDijkstraTC:
    """
    None
    """

    def is_build(self) -> bool:
        ...

class RangePHAST:
    """
    None
    """

    def is_build(self) -> bool:
        ...

class RangePHASTGS:
    """
    None
    """

    def is_build(self) -> bool:
        ...

class RangeRPHAST:
    """
    None
    """

    def is_build(self) -> bool:
        ...

class RangeRPHAST2:
    """
    None
    """

    def is_build(self) -> bool:
        ...

class RangeRPHASTGS:
    """
    None
    """

    def is_build(self) -> bool:
        ...

class Shortcut:
    """
    None
    """

    @overload
    def __init__(self, arg0: int, arg1: int, arg2: int, /) -> None:
        """
        __init__(self, arg0: int, arg1: int, arg2: int, /) -> None
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        __init__(self) -> None
        """
        ...

    @property
    def from_(self) -> int:
        ...
    @from_.setter
    def from_(self, arg: int, /) -> None:
        ...

    @property
    def to_(self) -> int:
        ...
    @to_.setter
    def to_(self, arg: int, /) -> None:
        ...

    @property
    def weight(self) -> int:
        ...
    @weight.setter
    def weight(self, arg: int, /) -> None:
        ...

class Snap:
    """
    None
    """

    @overload
    def __init__(self, arg0: int, arg1: int, /) -> None:
        """
        __init__(self, arg0: int, arg1: int, /) -> None
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        __init__(self) -> None
        """
        ...

    @property
    def dist(self) -> int:
        ...
    @dist.setter
    def dist(self, arg: int, /) -> None:
        ...

    @property
    def node(self) -> int:
        ...
    @node.setter
    def node(self, arg: int, /) -> None:
        ...

class SnapVector:
    """
    None
    """

    def __bool__(self) -> bool:
        """
        Check whether the vector is nonempty
        """
        ...

    @overload
    def __delitem__(self, arg: slice, /) -> None:
        """
        __delitem__(self, arg: slice, /) -> None
        """
        ...

    @overload
    def __delitem__(self, arg: int, /) -> None:
        """
        __delitem__(self, arg: int, /) -> None
        """
        ...

    @overload
    def __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.SnapVector:
        """
        __getitem__(self, arg: slice, /) -> pyaccess._pyaccess_ext.SnapVector
        """
        ...

    @overload
    def __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.DSnap:
        """
        __getitem__(self, arg: int, /) -> pyaccess._pyaccess_ext.DSnap
        """
        ...

    @overload
    def __init__(self, arg: collections.abc.Iterable[pyaccess._pyaccess_ext.DSnap], /) -> None:
        """
        Construct from an iterable object
        """
        ...

    @overload
    def __init__(self) -> None:
        """
        Default constructor
        """
        ...

    @overload
    def __init__(self, arg: pyaccess._pyaccess_ext.SnapVector) -> None:
        """
        Copy constructor
        """
        ...

    def __iter__(self) -> collections.abc.Iterator[pyaccess._pyaccess_ext.DSnap]:
        ...

    def __len__(self) -> int:
        ...

    def __repr__(self) -> str:
        ...

    @overload
    def __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.SnapVector, /) -> None:
        """
        __setitem__(self, arg0: slice, arg1: pyaccess._pyaccess_ext.SnapVector, /) -> None
        """
        ...

    @overload
    def __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.DSnap, /) -> None:
        """
        __setitem__(self, arg0: int, arg1: pyaccess._pyaccess_ext.DSnap, /) -> None
        """
        ...

    def append(self, arg: pyaccess._pyaccess_ext.DSnap, /) -> None:
        """
        Append `arg` to the end of the list.
        """
        ...

    def clear(self) -> None:
        """
        Remove all items from list.
        """
        ...

    def extend(self, arg: pyaccess._pyaccess_ext.SnapVector, /) -> None:
        """
        Extend `self` by appending elements from `arg`.
        """
        ...

    def insert(self, arg0: int, arg1: pyaccess._pyaccess_ext.DSnap, /) -> None:
        """
        Insert object `arg1` before index `arg0`.
        """
        ...

    def pop(self, index: int = -1) -> pyaccess._pyaccess_ext.DSnap:
        """
        Remove and return item at `index` (default last).
        """
        ...

class TCGraph(pyaccess._pyaccess_ext.IGraph):
    """
    None
    """
    ...

class TCWeighting:
    """
    None
    """

    def get_edge_weight(self, arg: int, /) -> int:
        ...

    def get_turn_cost(self, arg0: int, arg1: int, arg2: int, /) -> int:
        ...

    def set_edge_weight(self, arg0: int, arg1: int, /) -> None:
        ...

    def set_edge_weights(self, arg: numpy.typing.NDArray, /) -> None:
        ...

    def set_turn_cost(self, arg0: int, arg1: int, arg2: int, arg3: int, /) -> None:
        ...

class TiledData:
    """
    None
    """

    def get_adjacent_shortcuts(self, arg0: int, arg1: pyaccess._pyaccess_ext.Direction, /) -> list:
        ...

    def get_edge_type(self, arg: int, /) -> int:
        ...

    def get_shortcut(self, arg: int, /) -> pyaccess._pyaccess_ext.Shortcut:
        ...

    def shortcut_count(self) -> int:
        ...

class TiledGraph(pyaccess._pyaccess_ext.ITiledGraph, pyaccess._pyaccess_ext.IGraph):
    """
    None
    """
    ...

class TransitData:
    """
    None
    """

    def connection_count(self) -> int:
        ...

    def get_adjacent_connections(self, arg0: int, arg1: pyaccess._pyaccess_ext.Direction, /) -> list:
        ...

    def get_connection(self, arg: int, /) -> pyaccess._pyaccess_ext.Connection:
        ...

    def get_stop(self, arg: int, /) -> pyaccess._pyaccess_ext.Node:
        ...

    def has_node_stops(self, arg: int, /) -> bool:
        ...

    def map_node_to_stops(self, arg: int, /) -> list:
        ...

    def map_stop_to_node(self, arg: int, /) -> pyaccess._pyaccess_ext.DSnap:
        ...

    def stop_count(self) -> int:
        ...

class TransitDijkstra:
    """
    None
    """

    def is_build(self) -> bool:
        ...

    def set_max_departure(self, arg: int, /) -> None:
        ...

    def set_min_departure(self, arg: int, /) -> None:
        ...

class TransitGraph:
    """
    None
    """
    ...

class TransitWeighting:
    """
    None
    """

    def get_connection_weight(self, arg0: int, arg1: int, /) -> tuple[int, int] | None:
        ...

    def set_connection_schedule(self, arg0: int, arg1: collections.abc.Sequence[tuple[int, int]], /) -> None:
        ...

class Weighting:
    """
    None
    """

    def get_edge_weight(self, arg: int, /) -> int:
        ...

    def set_edge_weight(self, arg0: int, arg1: int, /) -> None:
        ...

    def set_edge_weights(self, arg: numpy.typing.NDArray, /) -> None:
        ...

def build_base_graph(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, /) -> pyaccess._pyaccess_ext.Graph:
    ...

def build_ch_graph(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.IDMapping, arg3: pyaccess._pyaccess_ext.CHData, arg4: pyaccess._pyaccess_ext.CHIndex, /) -> pyaccess._pyaccess_ext.CHGraph:
    ...

def build_ch_graph_2(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.Partition, arg3: pyaccess._pyaccess_ext.IDMapping, arg4: pyaccess._pyaccess_ext.CHData, arg5: pyaccess._pyaccess_ext.CHIndex, /) -> pyaccess._pyaccess_ext.CHGraph2:
    ...

def build_grasp_solver(arg: pyaccess._pyaccess_ext.ITiledGraph, /) -> pyaccess._pyaccess_ext.GRASP:
    ...

def build_range_dijkstra_solver(arg: pyaccess._pyaccess_ext.IGraph, /) -> pyaccess._pyaccess_ext.RangeDijkstra:
    ...

def build_range_dijkstra_tc_solver(arg: pyaccess._pyaccess_ext.IGraph, /) -> pyaccess._pyaccess_ext.RangeDijkstraTC:
    ...

def build_range_phast_gs_solver(arg: pyaccess._pyaccess_ext.CHGraph2, /) -> pyaccess._pyaccess_ext.RangePHASTGS:
    ...

def build_range_phast_solver(arg: pyaccess._pyaccess_ext.ICHGraph, /) -> pyaccess._pyaccess_ext.RangePHAST:
    ...

def build_range_rphast2_solver(arg: pyaccess._pyaccess_ext.ICHGraph, /) -> pyaccess._pyaccess_ext.RangeRPHAST2:
    ...

def build_range_rphast_gs_solver(arg: pyaccess._pyaccess_ext.CHGraph2, /) -> pyaccess._pyaccess_ext.RangeRPHASTGS:
    ...

def build_range_rphast_solver(arg: pyaccess._pyaccess_ext.ICHGraph, /) -> pyaccess._pyaccess_ext.RangeRPHAST:
    ...

def build_tc_graph(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.TCWeighting, /) -> pyaccess._pyaccess_ext.TCGraph:
    ...

def build_tiled_graph(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.Partition, arg3: pyaccess._pyaccess_ext.IDMapping, arg4: pyaccess._pyaccess_ext.TiledData, arg5: pyaccess._pyaccess_ext.CellIndex, /) -> pyaccess._pyaccess_ext.TiledGraph:
    ...

def build_transit_dijkstra_solver(arg: pyaccess._pyaccess_ext.TransitGraph, /) -> pyaccess._pyaccess_ext.TransitDijkstra:
    ...

def build_transit_graph(arg0: pyaccess._pyaccess_ext.IGraph, arg1: pyaccess._pyaccess_ext.TransitData, arg2: pyaccess._pyaccess_ext.TransitWeighting, /) -> pyaccess._pyaccess_ext.TransitGraph:
    ...

@overload
def calc_2sfca(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.typing.NDArray, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.typing.NDArray, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_2sfca(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.ndarray[dtype=int32, shape=(*)], arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.ndarray[dtype=int32, shape=(*)], arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.typing.NDArray, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.typing.NDArray, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.ndarray[dtype=int32, shape=(*)], arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.ndarray[dtype=int32, shape=(*)], arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_2sfca(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.typing.NDArray, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.typing.NDArray, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_2sfca(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.ndarray[dtype=int32, shape=(*)], arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.ndarray[dtype=int32, shape=(*)], arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.typing.NDArray, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.typing.NDArray, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.ndarray[dtype=int32, shape=(*)], arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.ndarray[dtype=int32, shape=(*)], arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.typing.NDArray, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.typing.NDArray, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.ndarray[dtype=int32, shape=(*)], arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.ndarray[dtype=int32, shape=(*)], arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_2sfca(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.typing.NDArray, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.typing.NDArray, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_2sfca(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.ndarray[dtype=int32, shape=(*)], arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.ndarray[dtype=int32, shape=(*)], arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.typing.NDArray, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.typing.NDArray, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.ndarray[dtype=int32, shape=(*)], arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.ndarray[dtype=int32, shape=(*)], arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_2sfca(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.typing.NDArray, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.typing.NDArray, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_2sfca(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.ndarray[dtype=int32, shape=(*)], arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.ndarray[dtype=int32, shape=(*)], arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.typing.NDArray, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.typing.NDArray, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_2sfca(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: numpy.ndarray[dtype=int32, shape=(*)], arg3: pyaccess._pyaccess_ext.SnapVector, arg4: numpy.ndarray[dtype=int32, shape=(*)], arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_aggregation(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.typing.NDArray, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_aggregation(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.ndarray[dtype=int32, shape=(*)], arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.typing.NDArray, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.ndarray[dtype=int32, shape=(*)], arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_aggregation(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.typing.NDArray, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_aggregation(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.ndarray[dtype=int32, shape=(*)], arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.typing.NDArray, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.ndarray[dtype=int32, shape=(*)], arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.typing.NDArray, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.ndarray[dtype=int32, shape=(*)], arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_aggregation(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.typing.NDArray, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_aggregation(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.ndarray[dtype=int32, shape=(*)], arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.typing.NDArray, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.ndarray[dtype=int32, shape=(*)], arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_aggregation(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.typing.NDArray, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_aggregation(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.ndarray[dtype=int32, shape=(*)], arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.typing.NDArray, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_aggregation(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: numpy.ndarray[dtype=int32, shape=(*)], arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

def calc_ch2_order(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Partition, arg2: pyaccess._pyaccess_ext.CHData, arg3: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_ch_order(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.CHData, arg2: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_crossing_penalties(arg: pyaccess._pyaccess_ext.GraphBase, /) -> numpy.typing.NDArray:
    ...

def calc_dfs_order(arg: pyaccess._pyaccess_ext.GraphBase, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_identity_order(arg0: pyaccess._pyaccess_ext.IDMapping, arg1: bool, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_mapped_order(arg0: pyaccess._pyaccess_ext.IntVector, arg1: pyaccess._pyaccess_ext.IDMapping, arg2: bool, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

@overload
def calc_matrix_query(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_matrix_query(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*, *), order='C']
    """
    ...

@overload
def calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*, *), order='C']
    """
    ...

@overload
def calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*, *), order='C']
    """
    ...

@overload
def calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*, *), order='C']
    """
    ...

@overload
def calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*, *), order='C']
    """
    ...

@overload
def calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*, *), order='C']
    """
    ...

@overload
def calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*, *), order='C']
    """
    ...

@overload
def calc_matrix_query(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_matrix_query(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*, *), order='C']
    """
    ...

@overload
def calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_matrix_query(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*, *), order='C']
    """
    ...

@overload
def calc_range_query(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_range_query(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*), order='C']
    """
    ...

@overload
def calc_range_query(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_range_query(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*), order='C']
    """
    ...

@overload
def calc_range_query(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_range_query(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*), order='C']
    """
    ...

@overload
def calc_range_query(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_range_query(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*), order='C']
    """
    ...

@overload
def calc_range_query(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_range_query(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*), order='C']
    """
    ...

@overload
def calc_range_query(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_range_query(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*), order='C']
    """
    ...

@overload
def calc_range_query(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_range_query(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*), order='C']
    """
    ...

@overload
def calc_range_query(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_range_query(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*), order='C']
    """
    ...

@overload
def calc_range_query(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.typing.NDArray:
    """
    calc_range_query(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.DSnap, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: int, /) -> numpy.ndarray[dtype=int32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_reachability(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_reachability(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_reachability(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_reachability(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_reachability(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_reachability(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_reachability(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_reachability(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.typing.NDArray:
    """
    calc_reachability(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> numpy.ndarray[dtype=float32, shape=(*), order='C']
    """
    ...

@overload
def calc_reachability_2(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    """
    calc_reachability_2(arg0: pyaccess._pyaccess_ext.TransitDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.ndarray[dtype=float32, shape=(*), order='C'], numpy.ndarray[dtype=int32, shape=(*), order='C']]
    """
    ...

@overload
def calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    """
    calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeDijkstra, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.ndarray[dtype=float32, shape=(*), order='C'], numpy.ndarray[dtype=int32, shape=(*), order='C']]
    """
    ...

@overload
def calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    """
    calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangePHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.ndarray[dtype=float32, shape=(*), order='C'], numpy.ndarray[dtype=int32, shape=(*), order='C']]
    """
    ...

@overload
def calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    """
    calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeRPHAST, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.ndarray[dtype=float32, shape=(*), order='C'], numpy.ndarray[dtype=int32, shape=(*), order='C']]
    """
    ...

@overload
def calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    """
    calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeRPHAST2, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.ndarray[dtype=float32, shape=(*), order='C'], numpy.ndarray[dtype=int32, shape=(*), order='C']]
    """
    ...

@overload
def calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    """
    calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangePHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.ndarray[dtype=float32, shape=(*), order='C'], numpy.ndarray[dtype=int32, shape=(*), order='C']]
    """
    ...

@overload
def calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    """
    calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeRPHASTGS, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.ndarray[dtype=float32, shape=(*), order='C'], numpy.ndarray[dtype=int32, shape=(*), order='C']]
    """
    ...

@overload
def calc_reachability_2(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    """
    calc_reachability_2(arg0: pyaccess._pyaccess_ext.GRASP, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.ndarray[dtype=float32, shape=(*), order='C'], numpy.ndarray[dtype=int32, shape=(*), order='C']]
    """
    ...

@overload
def calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    """
    calc_reachability_2(arg0: pyaccess._pyaccess_ext.RangeDijkstraTC, arg1: pyaccess._pyaccess_ext.SnapVector, arg2: pyaccess._pyaccess_ext.SnapVector, arg3: pyaccess._pyaccess_ext.IDistanceDecay, /) -> tuple[numpy.ndarray[dtype=float32, shape=(*), order='C'], numpy.ndarray[dtype=int32, shape=(*), order='C']]
    """
    ...

def calc_unconnected(arg: pyaccess._pyaccess_ext.GraphBase, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def edges_with_nodes(arg0: pyaccess._pyaccess_ext.IntVector, arg1: numpy.typing.NDArray, arg2: numpy.typing.NDArray, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def graph_bounds(arg: pyaccess._pyaccess_ext.NodeVector, /) -> tuple[float, float, float, float]:
    ...

def load_cell_index(arg: str, /) -> pyaccess._pyaccess_ext.CellIndex:
    ...

def load_ch_data(arg: str, /) -> pyaccess._pyaccess_ext.CHData:
    ...

def load_edge_weights(arg: str, /) -> pyaccess._pyaccess_ext.Weighting:
    ...

def load_graph_base(arg: str, /) -> pyaccess._pyaccess_ext.GraphBase:
    ...

def load_id_mapping(arg: str, /) -> pyaccess._pyaccess_ext.IDMapping:
    ...

def load_node_partition(arg: str, /) -> pyaccess._pyaccess_ext.Partition:
    ...

def load_tc_weights(arg: str, /) -> pyaccess._pyaccess_ext.TCWeighting:
    ...

def load_tiled_data(arg: str, /) -> pyaccess._pyaccess_ext.TiledData:
    ...

def load_transit_data(arg: str, /) -> pyaccess._pyaccess_ext.TransitData:
    ...

def load_transit_weights(arg: str, /) -> pyaccess._pyaccess_ext.TransitWeighting:
    ...

def new_graph_base(arg0: pyaccess._pyaccess_ext.NodeVector, arg1: pyaccess._pyaccess_ext.EdgeVector, /) -> pyaccess._pyaccess_ext.GraphBase:
    ...

def new_id_mapping(arg: int, /) -> pyaccess._pyaccess_ext.IDMapping:
    ...

def new_tc_weighting(arg: pyaccess._pyaccess_ext.GraphBase, /) -> pyaccess._pyaccess_ext.TCWeighting:
    ...

def new_transit_weighting(arg: pyaccess._pyaccess_ext.TransitData, /) -> pyaccess._pyaccess_ext.TransitWeighting:
    ...

def new_weighting(arg: pyaccess._pyaccess_ext.GraphBase, /) -> pyaccess._pyaccess_ext.Weighting:
    ...

def nodes_from_edges(arg: pyaccess._pyaccess_ext.EdgeVector, /) -> tuple[numpy.typing.NDArray, numpy.typing.NDArray]:
    ...

def parse_graph_from_osm(arg0: str, arg1: str, /) -> tuple[pyaccess._pyaccess_ext.NodeVector, pyaccess._pyaccess_ext.EdgeVector, numpy.typing.NDArray, numpy.typing.NDArray, numpy.typing.NDArray]:
    ...

def prepare_balanced_kdtree_index(arg: pyaccess._pyaccess_ext.GraphBase, /) -> pyaccess._pyaccess_ext.IGraphIndex:
    ...

def prepare_cell_index(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.Partition, /) -> pyaccess._pyaccess_ext.CellIndex:
    ...

def prepare_ch_index(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.CHData, arg3: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.CHIndex:
    ...

def prepare_ch_index_2(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.CHData, arg3: pyaccess._pyaccess_ext.Partition, arg4: pyaccess._pyaccess_ext.IDMapping, /) -> pyaccess._pyaccess_ext.CHIndex:
    ...

def prepare_ch_phast(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, /) -> pyaccess._pyaccess_ext.CHData:
    ...

def prepare_ch_simple(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, /) -> pyaccess._pyaccess_ext.CHData:
    ...

def prepare_ch_tiled(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.Partition, /) -> pyaccess._pyaccess_ext.CHData:
    ...

def prepare_isophast(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.Partition, /) -> tuple[pyaccess._pyaccess_ext.TiledData, pyaccess._pyaccess_ext.CellIndex]:
    ...

def prepare_kdtree_index(arg: pyaccess._pyaccess_ext.GraphBase, /) -> pyaccess._pyaccess_ext.IGraphIndex:
    ...

def prepare_partition(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: int, /) -> pyaccess._pyaccess_ext.Partition:
    ...

def prepare_tiled(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.Partition, /) -> pyaccess._pyaccess_ext.TiledData:
    ...

@overload
def prepare_transit(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.TCWeighting, arg2: pyaccess._pyaccess_ext.NodeVector, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: pyaccess._pyaccess_ext.ConnectionVector, arg5: int, /) -> pyaccess._pyaccess_ext.TransitData:
    """
    prepare_transit(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.TCWeighting, arg2: pyaccess._pyaccess_ext.NodeVector, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: pyaccess._pyaccess_ext.ConnectionVector, arg5: int, /) -> pyaccess._pyaccess_ext.TransitData
    """
    ...

@overload
def prepare_transit(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.NodeVector, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: pyaccess._pyaccess_ext.ConnectionVector, arg5: int, /) -> pyaccess._pyaccess_ext.TransitData:
    """
    prepare_transit(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.NodeVector, arg3: pyaccess._pyaccess_ext.SnapVector, arg4: pyaccess._pyaccess_ext.ConnectionVector, arg5: int, /) -> pyaccess._pyaccess_ext.TransitData
    """
    ...

def remove_nodes(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.GraphBase:
    ...

@overload
def reorder_nodes(arg0: pyaccess._pyaccess_ext.CellIndex, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.CellIndex:
    """
    reorder_nodes(arg0: pyaccess._pyaccess_ext.CellIndex, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.CellIndex
    """
    ...

@overload
def reorder_nodes(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.GraphBase:
    """
    reorder_nodes(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.GraphBase
    """
    ...

@overload
def reorder_nodes(arg0: pyaccess._pyaccess_ext.Weighting, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.Weighting:
    """
    reorder_nodes(arg0: pyaccess._pyaccess_ext.Weighting, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.Weighting
    """
    ...

@overload
def reorder_nodes(arg0: pyaccess._pyaccess_ext.TCWeighting, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.TCWeighting:
    """
    reorder_nodes(arg0: pyaccess._pyaccess_ext.TCWeighting, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.TCWeighting
    """
    ...

@overload
def reorder_nodes(arg0: pyaccess._pyaccess_ext.Partition, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.Partition:
    """
    reorder_nodes(arg0: pyaccess._pyaccess_ext.Partition, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.Partition
    """
    ...

@overload
def reorder_nodes(arg0: pyaccess._pyaccess_ext.CHData, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.CHData:
    """
    reorder_nodes(arg0: pyaccess._pyaccess_ext.CHData, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.CHData
    """
    ...

@overload
def reorder_nodes(arg0: pyaccess._pyaccess_ext.TiledData, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.TiledData:
    """
    reorder_nodes(arg0: pyaccess._pyaccess_ext.TiledData, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.TiledData
    """
    ...

@overload
def reorder_nodes(arg0: pyaccess._pyaccess_ext.CHIndex, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.CHIndex:
    """
    reorder_nodes(arg0: pyaccess._pyaccess_ext.CHIndex, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.CHIndex
    """
    ...

def reorder_sources(arg0: pyaccess._pyaccess_ext.IDMapping, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.IDMapping:
    ...

def reorder_targets(arg0: pyaccess._pyaccess_ext.IDMapping, arg1: pyaccess._pyaccess_ext.IntVector, /) -> pyaccess._pyaccess_ext.IDMapping:
    ...

def store_cell_index(arg0: pyaccess._pyaccess_ext.CellIndex, arg1: str, /) -> None:
    ...

def store_ch_data(arg0: pyaccess._pyaccess_ext.CHData, arg1: str, /) -> None:
    ...

def store_edge_weights(arg0: pyaccess._pyaccess_ext.Weighting, arg1: str, /) -> None:
    ...

def store_graph_base(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: str, /) -> None:
    ...

def store_id_mapping(arg0: pyaccess._pyaccess_ext.IDMapping, arg1: str, /) -> None:
    ...

def store_node_partition(arg0: pyaccess._pyaccess_ext.Partition, arg1: str, /) -> None:
    ...

def store_tc_weights(arg0: pyaccess._pyaccess_ext.TCWeighting, arg1: str, /) -> None:
    ...

def store_tiled_data(arg0: pyaccess._pyaccess_ext.TiledData, arg1: str, /) -> None:
    ...

def store_transit_data(arg0: pyaccess._pyaccess_ext.TransitData, arg1: str, /) -> None:
    ...

def store_transit_weights(arg0: pyaccess._pyaccess_ext.TransitWeighting, arg1: str, /) -> None:
    ...
