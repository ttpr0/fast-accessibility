from typing import Callable

from . import _pyaccess_ext


class Explorer:
    base: _pyaccess_ext.GraphBase
    weight: _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting
    index: _pyaccess_ext.IGraphIndex
    partition: _pyaccess_ext.Partition | None
    ch: _pyaccess_ext.CHData | None
    tiled: _pyaccess_ext.TiledData | None
    # TODO: missing id-mapping

    def __init__(self, base: _pyaccess_ext.GraphBase, weight: _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting, index: _pyaccess_ext.IGraphIndex | None = None, partition: _pyaccess_ext.Partition | None = None, ch: _pyaccess_ext.CHData | None = None, tiled: _pyaccess_ext.TiledData | None = None):
        self.base = base
        self.weight = weight
        if index is None:
            self.index = _pyaccess_ext.prepare_base_index(self.base)
        else:
            self.index = index
        self.partition = partition
        self.ch = ch
        self.tiled = tiled

    def node_count(self) -> int:
        return self.base.node_count()

    def edge_count(self) -> int:
        return self.base.edge_count()

    def get_node(self, node_id: int) -> _pyaccess_ext.Node:
        return self.base.get_node(node_id)

    def get_edge(self, edge_id: int) -> _pyaccess_ext.Edge:
        return self.base.get_edge(edge_id)

    def get_edge_weight(self, edge_id: int) -> int:
        return self.weight.get_edge_weight(edge_id)

    def get_turn_cost(self, from_edge_id: int, via_node_id: int, to_edge_id: int) -> int:
        if isinstance(self.weight, _pyaccess_ext.TCWeighting):
            return self.weight.get_turn_cost(from_edge_id, via_node_id, to_edge_id)
        else:
            return 0

    def get_closest_node(self, location: tuple[float, float]) -> int:
        coord = _pyaccess_ext.Coord(location[0], location[1])
        node, ok = self.index.get_closest_node(coord)
        if not ok:
            raise ValueError(f"no node near the given location (lon: {location[0]}, lat: {location[1]}) found")
        return node

    def get_adjacent_edges(self, node_id: int, direction: _pyaccess_ext.Direction) -> list[int]:
        edges = self.base.get_adjacent_edges(node_id, direction)
        return edges

    def get_node_level(self, node_id: int) -> int:
        if self.ch is None:
            raise ValueError("explorer does not contain node-level information")
        return self.ch.get_node_level(node_id)

    def get_node_partition(self, node_id: int) -> int:
        if self.partition is None:
            raise ValueError("explorer does not contain node-partition information")
        return self.partition.get_node_tile(node_id)

    def is_edge_overlay(self, edge_id: int) -> bool:
        if self.tiled is None:
            raise ValueError("explorer does not contain overlay information")
        return self.tiled.get_edge_type(edge_id) == 20

    def shortcut_count(self) -> int:
        if self.ch is not None:
            return self.ch.shortcut_count()
        if self.tiled is not None:
            return self.tiled.shortcut_count()
        raise ValueError("explorer does not contain shortcut information")

    def get_shortcut(self, shortcut_id: int) -> _pyaccess_ext.Shortcut:
        if self.ch is not None:
            return self.ch.get_shortcut(shortcut_id)
        if self.tiled is not None:
            return self.tiled.get_shortcut(shortcut_id)
        raise ValueError("explorer does not contain shortcut information")

    def get_adjacent_shortcuts(self, node_id: int, direction: _pyaccess_ext.Direction) -> list[int]:
        if self.ch is not None:
            return self.ch.get_adjacent_shortcuts(node_id, direction)
        if self.tiled is not None:
            return self.tiled.get_adjacent_shortcuts(node_id, direction)
        raise ValueError("explorer does not contain shortcut information")
