
class IntVector:
    ...


class FloatVector:
    ...


class CoordVector:
    ...


class Coord:
    lon: float
    lat: float

    def __init__(self):
        ...
    ...

class IGraph:
    ...


class Graph(IGraph):
    ...


class ICHGraph(IGraph):
    ...


class CHGraph(ICHGraph):
    ...

class CHGraph2(ICHGraph):
    ...


class ITiledGraph(IGraph):
    ...


class TiledGraph(ITiledGraph):
    ...


def load_graph(path: str) -> Graph:
    ...


def load_chgraph(path: str) -> CHGraph:
    ...

def load_chgraph_2(path: str) -> CHGraph2:
    ...

def load_tiled_graph(path: str) -> TiledGraph:
    ...


def calc_dijkstra_2sfca(graph: IGraph, dem_points: CoordVector, dem_weights: IntVector, sup_points: CoordVector, sup_weights: IntVector, max_range: int) -> FloatVector:
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

def calc_range_dijkstra(graph: IGraph, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...    

def calc_range_phast(graph: ICHGraph, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...    

def calc_range_phast_2(graph: CHGraph2, start_point: Coord, target_points: CoordVector, max_range: int) -> IntVector:
    ...    