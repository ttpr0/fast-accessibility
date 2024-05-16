

from .. import _pyaccess_ext
from ..builder import GraphBuilder, new_weighting, new_tc_weighting
from ..accessibility import calc_range, RANGE_DIJKSTRA, RANGE_PHAST, RANGE_RPHAST, RANGE_RPHAST_GS, GRASP

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
turn_penalties = [
    (13, 3, 7, 5),
    (32, 11, 36, 3)
]

def test_shortest_path():
    # build graph
    builder = GraphBuilder()
    try:
        for node in nodes:
            builder.add_node(node)
        for edge in edges:
            node_a, node_b, length = edge
            builder.add_edge(node_a, node_b)
            builder.add_edge(node_b, node_a)
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
    sup_point = (8, 6) # node 8
    dem_points = [
        (10, 3), # node 10
        (3, 1), # node 0
        (2, 8), # node 2
        (5, 9), # node 4
        (13, 3)  # node 13
    ]

    # compute shortest paths
    ranges = calc_range(graph, sup_point, dem_points, max_range=10)

    assert len(ranges) == 5
    assert ranges[0] == 4
    assert ranges[1] == 5
    assert ranges[2] == 7
    assert ranges[3] == -9999
    assert ranges[4] == 8

def test_shortest_path_tc():
    # build graph
    builder = GraphBuilder()
    try:
        for node in nodes:
            builder.add_node(node)
        for edge in edges:
            node_a, node_b, length = edge
            builder.add_edge(node_a, node_b)
            builder.add_edge(node_b, node_a)
    except:
        raise AssertionError("Error while building graph")
    graph = builder.build_graph()

    # build weighting
    weighting = new_tc_weighting(graph)
    for i in range(0, len(edges)):
        _, _, length = edges[i]
        weighting.set_edge_weight(2*i, length)
        weighting.set_edge_weight(2*i+1, length)
    for i in range(0, len(turn_penalties)):
        from_, via_, to_, penalty = turn_penalties[i]
        weighting.set_turn_cost(from_, via_, to_, penalty)
    graph.add_weighting("default", weighting)

    # test locations
    sup_point = (8, 6) # node 8
    dem_points = [
        (10, 3), # node 10
        (3, 1), # node 0
        (2, 8), # node 2
        (5, 9), # node 4
        (13, 3)  # node 13
    ]

    # compute shortest paths
    ranges = calc_range(graph, sup_point, dem_points, max_range=10)

    assert len(ranges) == 5
    assert ranges[0] == 4
    assert ranges[1] == 9
    assert ranges[2] == 7
    assert ranges[3] == -9999
    assert ranges[4] == 9

def test_contraction():
    # build graph
    builder = GraphBuilder()
    try:
        for node in nodes:
            builder.add_node(node)
        for edge in edges:
            node_a, node_b, length = edge
            builder.add_edge(node_a, node_b)
            builder.add_edge(node_b, node_a)
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

    # build contraction
    graph.add_contraction("ch", "default")

    # test locations
    sup_point = (8, 6) # node 8
    dem_points = [
        (10, 3), # node 10
        (3, 1), # node 0
        (2, 8), # node 2
        (5, 9), # node 4
        (13, 3)  # node 13
    ]

    # compute shortest paths using RangePHAST
    ranges = calc_range(graph, sup_point, dem_points, max_range=10, algorithm=RANGE_PHAST, ch="ch")

    assert len(ranges) == 5
    assert ranges[0] == 4
    assert ranges[1] == 5
    assert ranges[2] == 7
    assert ranges[3] == -9999
    assert ranges[4] == 8

    # compute shortest paths using RangeRPHAST
    ranges = calc_range(graph, sup_point, dem_points, max_range=10, algorithm=RANGE_RPHAST, ch="ch")

    assert len(ranges) == 5
    assert ranges[0] == 4
    assert ranges[1] == 5
    assert ranges[2] == 7
    assert ranges[3] == -9999
    assert ranges[4] == 8

def test_contraction_2():
    # build graph
    builder = GraphBuilder()
    try:
        for node in nodes:
            builder.add_node(node)
        for edge in edges:
            node_a, node_b, length = edge
            builder.add_edge(node_a, node_b)
            builder.add_edge(node_b, node_a)
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

    # build contraction
    graph.add_partition("partition", 3)
    graph.add_contraction("ch", "default", "partition")

    # test locations
    sup_point = (8, 6) # node 8
    dem_points = [
        (10, 3), # node 10
        (3, 1), # node 0
        (2, 8), # node 2
        (5, 9), # node 4
        (13, 3)  # node 13
    ]

    # compute shortest paths using RangePHAST+GS
    ranges = calc_range(graph, sup_point, dem_points, max_range=10, algorithm=RANGE_RPHAST_GS, ch="ch")

    assert len(ranges) == 5
    assert ranges[0] == 4
    assert ranges[1] == 5
    assert ranges[2] == 7
    assert ranges[3] == -9999
    assert ranges[4] == 8

def test_partition():
    # build graph
    builder = GraphBuilder()
    try:
        for node in nodes:
            builder.add_node(node)
        for edge in edges:
            node_a, node_b, length = edge
            builder.add_edge(node_a, node_b)
            builder.add_edge(node_b, node_a)
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

    # build partition
    graph.add_partition("partition", 3)
    graph.add_grasp_overlay("grasp", "default", "partition")

    # test locations
    sup_point = (8, 6) # node 8
    dem_points = [
        (10, 3), # node 10
        (3, 1), # node 0
        (2, 8), # node 2
        (5, 9), # node 4
        (13, 3)  # node 13
    ]

    # compute shortest paths using GRASP
    ranges = calc_range(graph, sup_point, dem_points, max_range=10, algorithm=GRASP, overlay="grasp")

    assert len(ranges) == 5
    assert ranges[0] == 4
    assert ranges[1] == 5
    assert ranges[2] == 7
    assert ranges[3] == -9999
    assert ranges[4] == 8

def test_partition_2():
    # build graph
    builder = GraphBuilder()
    try:
        for node in nodes:
            builder.add_node(node)
        for edge in edges:
            node_a, node_b, length = edge
            builder.add_edge(node_a, node_b)
            builder.add_edge(node_b, node_a)
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

    # build partition
    graph.add_partition("partition", 3)
    graph.add_isophast_overlay("isophast", "default", "partition")

    # test locations
    sup_point = (8, 6) # node 8
    dem_points = [
        (10, 3), # node 10
        (3, 1), # node 0
        (2, 8), # node 2
        (5, 9), # node 4
        (13, 3)  # node 13
    ]

    # compute shortest paths using GRASP
    ranges = calc_range(graph, sup_point, dem_points, max_range=10, algorithm=GRASP, overlay="isophast")

    assert len(ranges) == 5
    assert ranges[0] == 4
    assert ranges[1] == 5
    assert ranges[2] == 7
    assert ranges[3] == -9999
    assert ranges[4] == 8
