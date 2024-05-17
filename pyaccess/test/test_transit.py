import numpy as np

from .. import _pyaccess_ext
from ..builder import GraphBuilder
from ..graph import new_weighting, new_transit_weighting
from ..accessibility import calc_range

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
stops = [
    (3, 1), # 0
    (5, 3), # 5
    (10, 3), # 10
    (13, 3), # 13
    (6, 8), # 6
    (5, 9), # 4
]
conns = [
    (0, 1, 0),
    (1, 2, 0),
    (2, 3, 0),
    (3, 2, 1),
    (2, 1, 1),
    (1, 0, 1),
    (1, 4, 2),
    (4, 5, 2),
    (5, 4, 3),
    (4, 1, 3),
]
schedule = {
    0: [
        (10, 12, 0),
    ],
    1: [
        (12, 14, 0),
    ],
    2: [
        (14, 16, 0),
    ], # route 0 end
    3: [
        (10, 12, 0),
    ],
    4: [
        (12, 14, 0),
    ],
    5: [
        (14, 16, 0),
    ], # route 1 end
    6: [
        (15, 18, 0),
    ],
    7: [
        (18, 21, 0),
    ], # route 2 end
    8: [
        (4, 7, 0),
    ],
    9: [
        (7, 10, 0),
    ] # route 3 end
}

def test_transit_path():
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
    
    # build transit data
    stops_list = _pyaccess_ext.NodeVector()
    for stop in stops:
        s = _pyaccess_ext.Node()
        s.loc = _pyaccess_ext.Coord()
        s.loc.lon = stop[0]
        s.loc.lat = stop[1]
        stops_list.append(s)
    conns_list = _pyaccess_ext.ConnectionVector()
    for conn in conns:
        c = _pyaccess_ext.Connection(conn[0], conn[1], conn[2])
        conns_list.append(c)
    graph.add_public_transit("transit", stops_list, conns_list, "default", 5)
    transit_weight = new_transit_weighting(graph, "transit")
    for conn in schedule:
        trips = schedule[conn]
        t = []
        for trip in trips:
            t.append((trip[0], trip[1]))
        transit_weight.set_connection_schedule(conn, t)
    graph.add_transit_weighting("default", transit_weight, "transit")

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
    ranges = calc_range(graph, sup_point, dem_points, max_range=10, transit="transit", transit_weight="default", min_departure=8, max_departure=12)

    assert len(ranges) == 5
    assert ranges[0] == 4
    assert ranges[1] == 4
    assert ranges[2] == 7
    assert ranges[3] == 9
    assert ranges[4] == 6
