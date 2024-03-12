import json
import fiona
import pyaccess

GRAPH_DIR = "./data/graphs"
GRAPH_NAME = "test"
OUT_DIR = "."

graph = pyaccess.load_graph(GRAPH_NAME, GRAPH_DIR)
explorer = graph.get_explorer(partition="partition")

driver = "GPKG"

nodes_schema = {
    'geometry': 'Point',
    'properties': dict([
        ('index', 'int'),
        ('tile', 'int')
    ])
}

with fiona.open(f"{OUT_DIR}/graph.gpkg", "w", driver=driver, schema=nodes_schema, layer="nodes") as file:
    features = []
    for i in range(explorer.node_count()):
        node = explorer.get_node(i)
        coord = node.loc
        tile = explorer.get_node_partition(i)
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
        ('tile', 'int')
    ])
}

with fiona.open(f"{OUT_DIR}/graph.gpkg", "w", driver=driver, schema=edges_schema, layer="edges") as file:
    features = []
    for i in range(explorer.edge_count()):
        edge = explorer.get_edge(i)
        coord_a = explorer.get_node(edge.node_a).loc
        coord_b = explorer.get_node(edge.node_b).loc
        tile_a = explorer.get_node_partition(edge.node_a)
        tile_b = explorer.get_node_partition(edge.node_b)
        features.append({
            "geometry": {
                "type": "LineString",
                "coordinates": [[coord_a.lon, coord_a.lat], [coord_b.lon, coord_b.lat]],
            },
            "properties": {
                "index": i,
                "nodeA": edge.node_b,
                "nodeB": edge.node_b,
                "tile": tile_a if tile_a == tile_b else -1,
            }
        })
    file.writerecords(features)
