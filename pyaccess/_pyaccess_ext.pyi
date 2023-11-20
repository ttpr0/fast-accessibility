from typing import Any, Optional, overload, Sequence, Iterable, Iterator as iterator
from enum import Enum
import pyaccess._pyaccess_ext

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

class CHIndex2:
    """
    None
    """
    ...

class CellIndex:
    """
    None
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
    def __init__(self, arg: Iterable[pyaccess._pyaccess_ext.Coord], /) -> None:
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
    
    def __iter__(self) -> iterator:
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
    
class Edge:
    """
    None
    """

    @overload
    def __init__(self, arg0: int, arg1: int, arg2: pyaccess._pyaccess_ext.RoadType, arg3: float, arg4: int, /) -> None:
        """
        __init__(self, arg0: int, arg1: int, arg2: pyaccess._pyaccess_ext.RoadType, arg3: float, arg4: int, /) -> None
        """
        ...
    
    @overload
    def __init__(self) -> None:
        """
        __init__(self) -> None
        """
        ...
    
    @property
    def length(self) -> float:
        ...
    @length.setter
    def length(self, arg: float, /) -> None:
        ...
    
    @property
    def maxspeed(self) -> int:
        ...
    @maxspeed.setter
    def maxspeed(self, arg: int, /) -> None:
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
    
    @property
    def typ(self) -> pyaccess._pyaccess_ext.RoadType:
        ...
    @typ.setter
    def typ(self, arg: pyaccess._pyaccess_ext.RoadType, /) -> None:
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
    def __init__(self, arg: Iterable[pyaccess._pyaccess_ext.Edge], /) -> None:
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
    
    def __iter__(self) -> iterator:
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
    def __init__(self, arg: Iterable[float], /) -> None:
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
    
    def __iter__(self) -> iterator:
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
    
class Graph(pyaccess._pyaccess_ext.IGraph):
    """
    None
    """
    ...

class GraphBase:
    """
    None
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
    def __init__(self, arg: Iterable[int], /) -> None:
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
    
    def __iter__(self) -> iterator:
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

    def __init__(self) -> None:
        ...
    
    @property
    def typ(self) -> str:
        ...
    @typ.setter
    def typ(self, arg: str, /) -> None:
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
    def __init__(self, arg: Iterable[pyaccess._pyaccess_ext.Node], /) -> None:
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
    
    def __iter__(self) -> iterator:
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
    ...

class RoadType(Enum):
    """
    <attribute '__doc__' of 'RoadType' objects>
    """
    MOTORWAY = 1
    MOTORWAY_LINK = 2
    TRUNK = 3
    TRUNK_LINK = 4
    PRIMARY = 5
    PRIMARY_LINK = 6
    SECONDARY = 7
    SECONDARY_LINK = 8
    TERTIARY = 9
    TERTIARY_LINK = 10
    RESIDENTIAL = 11
    LIVING_STREET = 12
    UNCLASSIFIED = 13
    ROAD = 14
    TRACK = 15
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
    
class TiledData:
    """
    None
    """
    ...

class TiledGraph(pyaccess._pyaccess_ext.ITiledGraph, pyaccess._pyaccess_ext.IGraph):
    """
    None
    """
    ...

class Weighting:
    """
    None
    """
    ...

def build_base_graph(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, /) -> pyaccess._pyaccess_ext.Graph:
    ...

def build_ch_graph(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.CHData, arg3: pyaccess._pyaccess_ext.CHIndex, /) -> pyaccess._pyaccess_ext.CHGraph:
    ...

def build_ch_graph_2(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.Partition, arg3: pyaccess._pyaccess_ext.CHData, arg4: pyaccess._pyaccess_ext.CHIndex2, /) -> pyaccess._pyaccess_ext.CHGraph2:
    ...

def build_ch_index(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.CHData, /) -> pyaccess._pyaccess_ext.CHIndex:
    ...

def build_ch_index_2(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.CHData, arg3: pyaccess._pyaccess_ext.Partition, /) -> pyaccess._pyaccess_ext.CHIndex2:
    ...

def build_default_weighting(arg: pyaccess._pyaccess_ext.GraphBase, /) -> pyaccess._pyaccess_ext.Weighting:
    ...

def build_graph_base(arg0: pyaccess._pyaccess_ext.NodeVector, arg1: pyaccess._pyaccess_ext.EdgeVector, arg2: pyaccess._pyaccess_ext.CoordVector, /) -> pyaccess._pyaccess_ext.GraphBase:
    ...

def build_tiled_graph(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: pyaccess._pyaccess_ext.Weighting, arg2: pyaccess._pyaccess_ext.Partition, arg3: pyaccess._pyaccess_ext.TiledData, arg4: pyaccess._pyaccess_ext.CellIndex, /) -> pyaccess._pyaccess_ext.TiledGraph:
    ...

def calc_dijkstra_2sfca(arg0: pyaccess._pyaccess_ext.IGraph, arg1: pyaccess._pyaccess_ext.CoordVector, arg2: pyaccess._pyaccess_ext.IntVector, arg3: pyaccess._pyaccess_ext.CoordVector, arg4: pyaccess._pyaccess_ext.IntVector, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> pyaccess._pyaccess_ext.FloatVector:
    ...

def calc_dijkstra_reachability(arg0: pyaccess._pyaccess_ext.IGraph, arg1: pyaccess._pyaccess_ext.CoordVector, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: pyaccess._pyaccess_ext.IntVector, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> pyaccess._pyaccess_ext.FloatVector:
    ...

def calc_grasp(arg0: pyaccess._pyaccess_ext.ITiledGraph, arg1: pyaccess._pyaccess_ext.Coord, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: int, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_gs_phast(arg0: pyaccess._pyaccess_ext.CHGraph2, arg1: pyaccess._pyaccess_ext.Coord, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: int, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_gs_rphast(arg0: pyaccess._pyaccess_ext.CHGraph2, arg1: pyaccess._pyaccess_ext.Coord, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: int, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_range_dijkstra(arg0: pyaccess._pyaccess_ext.IGraph, arg1: pyaccess._pyaccess_ext.Coord, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: int, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_range_phast(arg0: pyaccess._pyaccess_ext.ICHGraph, arg1: pyaccess._pyaccess_ext.Coord, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: int, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_range_phast_2sfca(arg0: pyaccess._pyaccess_ext.ICHGraph, arg1: pyaccess._pyaccess_ext.CoordVector, arg2: pyaccess._pyaccess_ext.IntVector, arg3: pyaccess._pyaccess_ext.CoordVector, arg4: pyaccess._pyaccess_ext.IntVector, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> pyaccess._pyaccess_ext.FloatVector:
    ...

def calc_range_phast_reachability(arg0: pyaccess._pyaccess_ext.ICHGraph, arg1: pyaccess._pyaccess_ext.CoordVector, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: pyaccess._pyaccess_ext.IntVector, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> pyaccess._pyaccess_ext.FloatVector:
    ...

def calc_range_rphast(arg0: pyaccess._pyaccess_ext.ICHGraph, arg1: pyaccess._pyaccess_ext.Coord, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: int, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_range_rphast2_reachability(arg0: pyaccess._pyaccess_ext.ICHGraph, arg1: pyaccess._pyaccess_ext.CoordVector, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: pyaccess._pyaccess_ext.IntVector, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> pyaccess._pyaccess_ext.FloatVector:
    ...

def calc_range_rphast_2(arg0: pyaccess._pyaccess_ext.ICHGraph, arg1: pyaccess._pyaccess_ext.Coord, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: int, /) -> pyaccess._pyaccess_ext.IntVector:
    ...

def calc_range_rphast_2sfca(arg0: pyaccess._pyaccess_ext.ICHGraph, arg1: pyaccess._pyaccess_ext.CoordVector, arg2: pyaccess._pyaccess_ext.IntVector, arg3: pyaccess._pyaccess_ext.CoordVector, arg4: pyaccess._pyaccess_ext.IntVector, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> pyaccess._pyaccess_ext.FloatVector:
    ...

def calc_range_rphast_2sfca2(arg0: pyaccess._pyaccess_ext.ICHGraph, arg1: pyaccess._pyaccess_ext.CoordVector, arg2: pyaccess._pyaccess_ext.IntVector, arg3: pyaccess._pyaccess_ext.CoordVector, arg4: pyaccess._pyaccess_ext.IntVector, arg5: pyaccess._pyaccess_ext.IDistanceDecay, /) -> pyaccess._pyaccess_ext.FloatVector:
    ...

def calc_range_rphast_reachability(arg0: pyaccess._pyaccess_ext.ICHGraph, arg1: pyaccess._pyaccess_ext.CoordVector, arg2: pyaccess._pyaccess_ext.CoordVector, arg3: pyaccess._pyaccess_ext.IntVector, arg4: pyaccess._pyaccess_ext.IDistanceDecay, /) -> pyaccess._pyaccess_ext.FloatVector:
    ...

def load_cell_index(arg: str, /) -> pyaccess._pyaccess_ext.CellIndex:
    ...

def load_ch_data(arg: str, /) -> pyaccess._pyaccess_ext.CHData:
    ...

def load_edge_weights(arg: str, /) -> pyaccess._pyaccess_ext.Weighting:
    ...

def load_graph_base(arg: str, /) -> pyaccess._pyaccess_ext.GraphBase:
    ...

def load_node_partition(arg: str, /) -> pyaccess._pyaccess_ext.Partition:
    ...

def load_tiled_data(arg0: str, arg1: int, /) -> pyaccess._pyaccess_ext.TiledData:
    ...

def store_cell_index(arg0: pyaccess._pyaccess_ext.CellIndex, arg1: str, /) -> None:
    ...

def store_ch_data(arg0: pyaccess._pyaccess_ext.CHData, arg1: str, /) -> None:
    ...

def store_edge_weights(arg0: pyaccess._pyaccess_ext.Weighting, arg1: str, /) -> None:
    ...

def store_graph_base(arg0: pyaccess._pyaccess_ext.GraphBase, arg1: str, /) -> None:
    ...

def store_node_partition(arg0: pyaccess._pyaccess_ext.Partition, arg1: str, /) -> None:
    ...

def store_tiled_data(arg0: pyaccess._pyaccess_ext.TiledData, arg1: str, arg2: int, /) -> None:
    ...

