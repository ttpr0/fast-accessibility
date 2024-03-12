from .. import _pyaccess_ext
from ..builder import GraphBuilder, new_weighting
from ..accessibility import calc_2sfca, calc_matrix, OneToManyType

nodes = [
    (3, 1),
    (1, 4),
    (2, 8),
    (4, 6),
    (5, 9),
    (5, 3),
    (6, 8),
    (7, 1),
    (8, 6),
    (10, 9),
    (10, 3),
    (11, 7),
    (11, 1),
    (13, 3),
]
edges = [
    (0, 1, 3),
    (1, 2, 2),
    (2, 3, 4),
    (0, 3, 2),
    (3, 4, 8),
    (3, 6, 4),
    (3, 5, 1),
    (0, 7, 6),
    (5, 8, 2),
    (5, 7, 1),
    (6, 8, 3),
    (4, 9, 5),
    (8, 9, 9),
    (8, 10, 4),
    (7, 10, 4),
    (9, 11, 2),
    (10, 11, 2),
    (10, 12, 2),
    (11, 13, 2),
    (12, 13, 3)
]

def test_matrix():
    # build graph
    builder = GraphBuilder()
    try:
        for node in nodes:
            builder.add_node(node)
        for edge in edges:
            node_a, node_b, length = edge
            builder.add_edge(node_a, node_b, length, 30, _pyaccess_ext.RoadType.ROAD)
            builder.add_edge(node_b, node_a, length, 30, _pyaccess_ext.RoadType.ROAD)
    except:
        raise AssertionError("Error while building graph")
    graph = builder.build_graph()

    # build weighting
    weighting = new_weighting(graph)
    for i in range(0, len(edges)):
        _, _, length = edges[i]
        weighting.set_edge_weight(2*i, length)
        weighting.set_edge_weight(2*i+1, length)
    graph.add_weighting("default", weighting)

    # test locations
    sup_points = [
        (8, 6), # node 8
        (4, 6), # node 3
        (7, 1), # node 7
    ]
    dem_points = [
        (10, 3), # node 10
        (3, 1), # node 0
        (2, 8), # node 2
        (5, 9), # node 4
        (13, 3)  # node 13
    ]

    # compute shortest paths
    mat = calc_matrix(graph, sup_points, dem_points, max_range=10)

    assert mat.shape == (3, 5)
    assert mat[0, 0] == 4
    assert mat[2, 2] == 6
    assert mat[0, 3] == -9999
    assert mat[2, 4] == 8
    assert mat[2, 3] == 10

def test_enhanced_2sfca():
    # build graph
    builder = GraphBuilder()
    try:
        for node in nodes:
            builder.add_node(node)
        for edge in edges:
            node_a, node_b, length = edge
            builder.add_edge(node_a, node_b, length, 30, _pyaccess_ext.RoadType.ROAD)
            builder.add_edge(node_b, node_a, length, 30, _pyaccess_ext.RoadType.ROAD)
    except:
        raise AssertionError("Error while building graph")
    graph = builder.build_graph()

    # build weighting
    weighting = new_weighting(graph)
    for i in range(0, len(edges)):
        _, _, length = edges[i]
        weighting.set_edge_weight(2*i, length)
        weighting.set_edge_weight(2*i+1, length)
    graph.add_weighting("default", weighting)

    # test locations
    sup_points = [
        (8, 6), # node 8
        (4, 6), # node 3
        (7, 1), # node 7
    ]
    sup_weights = [
        20, 25, 15
    ]
    dem_points = [
        (10, 3), # node 10
        (3, 1), # node 0
        (2, 8), # node 2
        (5, 9), # node 4
        (13, 3)  # node 13
    ]
    dem_weights = [
        10, 15, 12, 20, 5
    ]
    decay = _pyaccess_ext.HybridDecay(_pyaccess_ext.IntVector([2, 5, 10]), _pyaccess_ext.FloatVector([1.0, 0.5, 0.2]))

    # compute shortest paths
    access = calc_2sfca(graph, dem_points, dem_weights, sup_points, sup_weights, decay=decay)

    assert len(access) == 5
    assert abs(access[0] - 1.184) <= 0.001
    assert abs(access[1] - 1.899) <= 0.001
    assert abs(access[2] - 0.849) <= 0.001
    assert abs(access[3] - 0.330) <= 0.001
    assert abs(access[4] - 0.581) <= 0.001
