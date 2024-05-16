from __future__ import annotations
from typing import Any
from enum import Enum, IntEnum
import os
import geopandas as gpd

from .. import _pyaccess_ext
from . import Ordering


class BaseObject:
    extent: tuple[float, float, float, float]
    has_attr: bool

    base: _pyaccess_ext.GraphBase | None
    index: _pyaccess_ext.IGraphIndex | None
    node_attr: gpd.GeoDataFrame | None
    edge_attr: gpd.GeoDataFrame | None

    has_changed: bool

    def __init__(self, extent: tuple[float, float, float, float], has_attr: bool, base: _pyaccess_ext.GraphBase | None = None, index: _pyaccess_ext.IGraphIndex | None = None, node_attr: gpd.GeoDataFrame | None = None, edge_attr: gpd.GeoDataFrame | None = None):
        self.extent = extent
        self.has_attr = has_attr

        if base is None:
            self.has_changed = False
        else:
            self.has_changed = True

        self.base = base
        self.index = index
        self.node_attr = node_attr
        self.edge_attr = edge_attr

    def load(self, path: str):
        if not os.path.isfile(f"{path}-graph"):
            raise NotImplementedError("unable to find base-object")
        if not os.path.isfile(f"{path}-nodes") or not os.path.isfile(f"{path}-edges"):
            raise NotImplementedError("unable to find attribute-objects")
        if self.base is None:
            self.base = _pyaccess_ext.load_graph_base(f"{path}")
            if self.has_attr:
                self.node_attr = gpd.read_feather(f"{path}-nodes")
                self.edge_attr = gpd.read_feather(f"{path}-edges")
            self.has_changed = False
    
    def is_loaded(self) -> bool:
        if self.base is None:
            return False
        return True

    def store(self, path: str):
        if self.base is None:
            raise NotImplementedError("storing unloaded base-object not possibile")
        if not self.has_changed:
            return
        _pyaccess_ext.store_graph_base(self.base, f"{path}")
        if self.has_attr and self.node_attr is not None and self.edge_attr is not None:
            self.node_attr.to_feather(f"{path}-nodes")
            self.edge_attr.to_feather(f"{path}-edges")
        self.has_changed = False

    def delete(self, path: str):
        if os.path.isfile(f"{path}-graph"):
            os.remove(f"{path}-graph")
        if os.path.isfile(f"{path}-nodes"):
            os.remove(f"{path}-nodes")
        if os.path.isfile(f"{path}-edges"):
            os.remove(f"{path}-edges")
        self.base = None
        self.index = None
        self.node_attr = None
        self.edge_attr = None

    def get_base(self) -> _pyaccess_ext.GraphBase:
        if self.base is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.base

    def has_index(self) -> bool:
        return self.index is not None

    def build_index(self):
        if self.base is None:
            raise NotImplementedError("this should not have happened, please load first")
        # self.index = _pyaccess_ext.prepare_kdtree_index(self.base)
        self.index = _pyaccess_ext.prepare_balanced_kdtree_index(self.base)

    def get_index(self) -> _pyaccess_ext.IGraphIndex:
        if self.index is None:
            raise NotImplementedError("this should not have happened, please build first")
        return self.index

    def get_node_attr(self) -> gpd.GeoDataFrame | None:
        if self.node_attr is None:
            if self.has_attr:
                raise NotImplementedError("this should not have happened, please load first")
            else:
                return None
        return self.node_attr

    def get_edge_attr(self) -> gpd.GeoDataFrame | None:
        if self.edge_attr is None:
            if self.has_attr:
                raise NotImplementedError("this should not have happened, please load first")
            else:
                return None
        return self.edge_attr

    def get_metadata(self) -> Any:
        return {
            "extent": self.extent,
            "has_attr": self.has_attr
        }

    def reorder(self, ordering: Ordering, mapping: _pyaccess_ext.IntVector):
        """reorders nodes
        """
        if self.base is None:
            raise NotImplementedError("unable to reorder unloaded base-object")
        self.base = _pyaccess_ext.reorder_nodes(self.base, mapping)
        self.index = None
        if self.has_attr:
            if self.node_attr is None:
                raise NotImplementedError("unable to reorder unloaded node-attributes")
            self.node_attr["_sorter"] = mapping
            self.node_attr.sort_values("_sorter", inplace=True, ignore_index=True)
            del self.node_attr["_sorter"]
        self.has_changed = True

    def remove_unconnected(self):
        """removes nodes not part of the largest connected component
        """
        if self.base is None:
            raise NotImplementedError("unable to reorder unloaded base-object")
        remove_nodes = _pyaccess_ext.calc_unconnected(self.base)
        # get all edges that connect to the removed nodes
        node_set = set(remove_nodes)
        remove_edges = []
        for i in range(self.base.edge_count()):
            edge = self.base.get_edge(i)
            if edge.node_a in node_set or edge.node_b in node_set:
                remove_edges.append(i)
        # update components
        self.base = _pyaccess_ext.remove_nodes(self.base, remove_nodes)
        self.index = None
        if self.has_attr:
            if self.node_attr is None or self.edge_attr is None:
                raise NotImplementedError("unable to reorder unloaded node-attributes")
            self.node_attr.drop(remove_nodes, inplace=True)
            self.node_attr.reset_index(drop=True, inplace=True)
            self.edge_attr.drop(remove_edges, inplace=True)
            self.edge_attr.reset_index(drop=True, inplace=True)
        self.has_changed = True

def BaseObject_from_metadata(meta: dict[str, Any]) -> BaseObject:
    extent: Any = tuple(meta["extent"])
    has_attr: bool = meta["has_attr"]
    obj = BaseObject(extent, has_attr)
    return obj

def BaseObject_new(nodes: _pyaccess_ext.NodeVector, edges: _pyaccess_ext.EdgeVector, node_attr: gpd.GeoDataFrame | None = None, edge_attr: gpd.GeoDataFrame | None = None) -> BaseObject:
    base = _pyaccess_ext.new_graph_base(nodes, edges)
    minx, miny, maxx, maxy = 1000000, 1000000, -1000000, -1000000
    for i in range(len(nodes)):
        node = nodes[i]
        x = node.loc.lon
        y = node.loc.lat
        minx = min(minx, x)
        miny = min(miny, y)
        maxx = max(maxx, x)
        maxy = max(maxy, y)
    if node_attr is not None and edge_attr is not None:
        obj = BaseObject((minx, miny, maxx, maxy), True, base=base, node_attr=node_attr, edge_attr=edge_attr)
    else:
        obj = BaseObject((minx, miny, maxx, maxy), False, base=base)
    return obj
