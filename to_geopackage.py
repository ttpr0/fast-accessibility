import json
import fiona
import pyaccess

base = pyaccess.load_graph_base("./data/graphs/base")
partition = pyaccess.load_node_partition("./data/graphs/partition")

driver = "GPKG"

nodes_schema = {
    'geometry': 'Point',
    'properties': dict([
        ('index', 'int'),
        ('tile', 'int')
    ])
}

with fiona.open("graph.gpkg", "w", driver=driver, schema=nodes_schema, layer="nodes") as file:
    features = []
    for i in range(base.node_count()):
        node = base.get_node(i)
        coord = base.get_node_geom(i)
        tile = partition.get_node_tile(i)
        features.append({
            "geometry": {
                "type": "Point",
                "coordinates": [coord.lon, coord.lat],
            },
            "properties": {
                "index": i,
                "tile": tile,
            }
        })
    file.writerecords(features)

edges_schema = {
    'geometry': 'LineString',
    'properties': dict([
        ('index', 'int'),
        ('nodeA', 'int'),
        ('nodeB', 'int'),
        ('type', 'int')
    ])
}

with fiona.open("graph.gpkg", "w", driver=driver, schema=edges_schema, layer="edges") as file:
    features = []
    for i in range(base.edge_count()):
        edge = base.get_edge(i)
        coord_a = base.get_node_geom(edge.node_a)
        coord_b = base.get_node_geom(edge.node_b)
        features.append({
            "geometry": {
                "type": "LineString",
                "coordinates": [[coord_a.lon, coord_a.lat], [coord_b.lon, coord_b.lat]],
            },
            "properties": {
                "index": i,
                "nodeA": edge.node_b,
                "nodeB": edge.node_b,
                "type": 0,
            }
        })
    file.writerecords(features)
