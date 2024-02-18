from typing import Callable

from . import _pyaccess_ext


class Explorer:
    _base: _pyaccess_ext.GraphBase
    _weight: _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting
    _index: _pyaccess_ext.IGraphIndex
    _partition: _pyaccess_ext.Partition | None
    _id_mapping: _pyaccess_ext.IDMapping | None
    _ch: _pyaccess_ext.CHData | None
    _tiled: _pyaccess_ext.TiledData | None

    def __init__(self, base: _pyaccess_ext.GraphBase, weight: _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting, index: _pyaccess_ext.IGraphIndex | None = None, partition: _pyaccess_ext.Partition | None = None, id_mapping: _pyaccess_ext.IDMapping | None = None, ch: _pyaccess_ext.CHData | None = None, tiled: _pyaccess_ext.TiledData | None = None):
        self._base = base
        self._weight = weight
        if index is None:
            self._index = _pyaccess_ext.prepare_base_index(self._base)
        else:
            self._index = index
        self._partition = partition
        self._id_mapping = id_mapping
        if ch is not None and tiled is not None:
            raise ValueError("explorer cannot contain both CH and tiled data")
        self._ch = ch
        self._tiled = tiled

    def node_count(self) -> int:
        return self._base.node_count()

    def edge_count(self) -> int:
        return self._base.edge_count()

    def get_node(self, node_id: int) -> _pyaccess_ext.Node:
        return self._base.get_node(node_id)

    def get_edge(self, edge_id: int) -> _pyaccess_ext.Edge:
        return self._base.get_edge(edge_id)

    def get_edge_weight(self, edge_id: int) -> int:
        return self._weight.get_edge_weight(edge_id)

    def get_turn_cost(self, from_edge_id: int, via_node_id: int, to_edge_id: int) -> int:
        if isinstance(self._weight, _pyaccess_ext.TCWeighting):
            return self._weight.get_turn_cost(from_edge_id, via_node_id, to_edge_id)
        else:
            return 0

    def get_closest_node(self, location: tuple[float, float]) -> int:
        coord = _pyaccess_ext.Coord(location[0], location[1])
        node, ok = self._index.get_closest_node(coord)
        if not ok:
            raise ValueError(f"no node near the given location (lon: {location[0]}, lat: {location[1]}) found")
        return node

    def get_adjacent_edges(self, node_id: int, direction: _pyaccess_ext.Direction) -> list[int]:
        edges = self._base.get_adjacent_edges(node_id, direction)
        return edges

    def get_node_level(self, node_id: int) -> int:
        if self._ch is None or self._id_mapping is None:
            raise ValueError("explorer does not contain node-level information")
        m_node_id = self._id_mapping.get_target(node_id)
        return self._ch.get_node_level(m_node_id)

    def get_node_partition(self, node_id: int) -> int:
        if self._partition is None:
            raise ValueError("explorer does not contain node-partition information")
        return self._partition.get_node_tile(node_id)

    def is_edge_overlay(self, edge_id: int) -> bool:
        if self._tiled is None:
            raise ValueError("explorer does not contain overlay information")
        return self._tiled.get_edge_type(edge_id) == 20

    def shortcut_count(self) -> int:
        if self._ch is not None:
            return self._ch.shortcut_count()
        if self._tiled is not None:
            return self._tiled.shortcut_count()
        raise ValueError("explorer does not contain shortcut information")

    def get_shortcut(self, shortcut_id: int) -> _pyaccess_ext.Shortcut:
        if self._ch is not None:
            return self._ch.get_shortcut(shortcut_id)
        if self._tiled is not None:
            return self._tiled.get_shortcut(shortcut_id)
        raise ValueError("explorer does not contain shortcut information")

    def get_adjacent_shortcuts(self, node_id: int, direction: _pyaccess_ext.Direction) -> list[int]:
        if self._ch is not None and self._id_mapping is not None:
            m_node_id = self._id_mapping.get_target(node_id)
            return self._ch.get_adjacent_shortcuts(m_node_id, direction)
        if self._tiled is not None and self._id_mapping is not None:
            m_node_id = self._id_mapping.get_target(node_id)
            return self._tiled.get_adjacent_shortcuts(m_node_id, direction)
        raise ValueError("explorer does not contain shortcut information")
