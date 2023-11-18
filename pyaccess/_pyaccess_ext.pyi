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
    
class Partition:
    """
    None
    """
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

def load_chgraph(arg: str, /) -> pyaccess._pyaccess_ext.CHGraph:
    ...

def load_chgraph_2(arg: str, /) -> pyaccess._pyaccess_ext.CHGraph2:
    ...

def load_edge_weights(arg: str, /) -> pyaccess._pyaccess_ext.Weighting:
    ...

def load_graph(arg: str, /) -> pyaccess._pyaccess_ext.Graph:
    ...

def load_graph_base(arg: str, /) -> pyaccess._pyaccess_ext.GraphBase:
    ...

def load_node_partition(arg: str, /) -> pyaccess._pyaccess_ext.Partition:
    ...

def load_tiled_data(arg0: str, arg1: int, /) -> pyaccess._pyaccess_ext.TiledData:
    ...

def load_tiled_graph(arg: str, /) -> pyaccess._pyaccess_ext.TiledGraph:
    ...

