
class IntVector:
    ...


class FloatVector:
    ...


class CoordVector:
    ...


class Coord:
    def lon(self) -> float:
        ...

    def lat(self) -> float:
        ...


class IGraph:
    ...


class Graph(IGraph):
    ...


class ICHGraph(IGraph):
    ...


class CHGraph(ICHGraph):
    ...


def load_graph(path: str) -> Graph:
    ...


def load_chgraph(path: str) -> CHGraph:
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
