
#**************************************
# utility methods
#**************************************

class IntVector:
    ...

class FloatVector:
    ...

class CoordVector:
    def append(self, coord: tuple[float, float]) -> None:
        ...
    ...

class Coord:
    lon: float
    lat: float

    def __init__(self):
        ...
    ...

#**************************************
# graph classes
#**************************************

class GraphBase:
    ...

class Weighting:
    ...

class Partition:
    ...

class CHData:
    ...

class _CHIndex:
    ...

class _CHIndex2:
    ...

class TiledData:
    ...

class _CellIndex:
    ...


class IGraph:
    ...

class Graph(IGraph):
    ...

class ICHGraph(IGraph):
    ...

class CHGraph(ICHGraph):
    ...

class CHGraph2(IGraph):
    ...

class ITiledGraph(IGraph):
    ...

class TiledGraph(ITiledGraph):
    ...

#**************************************
# graph utility methods
#**************************************

def load_graph_base(file: str) -> GraphBase:
    ...

def load_edge_weights(file: str) -> Weighting:
    ...

def load_node_partition(file: str) -> Partition:
    ...

def load_ch_data(file: str) ->CHData:
    ...

def load_tiled_data(file: str, nodecount: int) -> TiledData:
    ...

def load_cell_index(file: str) -> _CellIndex:
    ...


def build_base_graph(base: GraphBase, weight: Weighting) -> Graph:
    ...

def build_tiled_graph(base: GraphBase, weight: Weighting, partition: Partition, tiled: TiledData, index: _CellIndex) -> TiledGraph:
    ...

def build_ch_graph(base: GraphBase, weight: Weighting, ch: CHData, index: _CHIndex) -> CHGraph:
    ...

def build_ch_graph_2(base: GraphBase, weight: Weighting, partition: Partition, ch: CHData, index: _CHIndex2) -> CHGraph2:
    ...


def build_ch_index(base: GraphBase, weight: Weighting, ch: CHData) -> _CHIndex:
    ...

def build_ch_index_2(base: GraphBase, weight: Weighting, ch: CHData, partition: Partition) -> _CHIndex2:
    ...


def load_graph(path: str) -> Graph:
    ...

def load_chgraph(path: str) -> CHGraph:
    ...

def load_chgraph_2(path: str) -> CHGraph2:
    ...

def load_tiled_graph(path: str) -> TiledGraph:
    ...


#**************************************
# accessibility functions
#**************************************

# 2sfca functions

def calc_dijkstra_2sfca(graph: IGraph, dem_points: CoordVector, dem_weights: IntVector, sup_points: CoordVector, sup_weights: IntVector, max_range: int) -> FloatVector:
    ...

def calc_dijkstra_2sfca_2(graph: IGraph, dem_points: CoordVector, dem_weights: IntVector, sup_points: CoordVector, sup_weights: IntVector, max_range: int) -> FloatVector:
    ...

def calc_range_phast_2sfca(graph: ICHGraph, dem_points: CoordVector, dem_weights: IntVector, sup_points: CoordVector, sup_weights: IntVector, max_range: int) -> FloatVector:
    ...

def calc_range_rphast_2sfca(graph: ICHGraph, dem_points: CoordVector, dem_weights: IntVector, sup_points: CoordVector, sup_weights: IntVector, max_range: int) -> FloatVector:
    ...

def calc_range_rphast_2sfca2(graph: ICHGraph, dem_points: CoordVector, dem_weights: IntVector, sup_points: CoordVector, sup_weights: IntVector, max_range: int) -> FloatVector:
    ...

def calc_range_rphast_2sfca3(graph: ICHGraph, dem_points: CoordVector, dem_weights: IntVector, sup_points: CoordVector, sup_weights: IntVector, max_range: int) -> FloatVector:
    ...

def calc_tiled_2sfca(graph: ITiledGraph, dem_points: CoordVector, dem_weights: IntVector, sup_points: CoordVector, sup_weights: IntVector, max_range: int) -> FloatVector:
    ...

def calc_tiled_2sfca2(graph: ITiledGraph, dem_points: CoordVector, dem_weights: IntVector, sup_points: CoordVector, sup_weights: IntVector, max_range: int) -> FloatVector:
    ...


# range query functions

def calc_range_dijkstra(graph: IGraph, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...

def calc_range_phast(graph: ICHGraph, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...

def calc_range_rphast(graph: ICHGraph, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...

def calc_range_rphast_2(graph: ICHGraph, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...

def calc_gs_phast(graph: CHGraph2, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...

def calc_gs_rphast(graph: CHGraph2, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...

def calc_grasp(graph: ITiledGraph, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...
