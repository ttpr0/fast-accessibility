from __future__ import annotations
from typing import Any
from enum import Enum, IntEnum
import os

from .. import _pyaccess_ext
from . import Ordering


class BaseObject:
    extent: tuple[float, float, float, float]

    base: _pyaccess_ext.GraphBase | None
    index: _pyaccess_ext.IGraphIndex | None

    has_changed: bool

    def __init__(self, extent: tuple[float, float, float, float], base: _pyaccess_ext.GraphBase | None = None, index: _pyaccess_ext.IGraphIndex | None = None):
        self.extent = extent

        if base is None:
            self.has_changed = False
        else:
            self.has_changed = True

        self.base = base
        self.index = index

    def load(self, path: str):
        if not os.path.isfile(f"{path}-nodes") or not os.path.isfile(f"{path}-edges"):
            raise NotImplementedError("unable to find base-object")
        if self.base is None:
            self.base = _pyaccess_ext.load_graph_base(f"{path}")
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
        self.has_changed = False

    def delete(self, path: str):
        if not os.path.isfile(f"{path}-nodes"):
            os.remove(f"{path}-nodes")
        if not os.path.isfile(f"{path}-edges"):
            os.remove(f"{path}-edges")
        self.base = None
        self.index = None

    def get_base(self) -> _pyaccess_ext.GraphBase:
        if self.base is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.base

    def has_index(self) -> bool:
        return self.index is not None

    def build_index(self):
        if self.base is None:
            raise NotImplementedError("this should not have happened, please load first")
        self.index = _pyaccess_ext.prepare_kdtree_index(self.base)

    def get_index(self) -> _pyaccess_ext.IGraphIndex:
        if self.index is None:
            raise NotImplementedError("this should not have happened, please build first")
        return self.index

    def get_metadata(self) -> Any:
        return {
            "extent": self.extent,
        }

    def reorder(self, ordering: Ordering, mapping: _pyaccess_ext.IntVector):
        """reorders nodes
        """
        if self.base is None:
            raise NotImplementedError("unable to reorder unloaded base-object")
        self.base = _pyaccess_ext.reorder_nodes(self.base, mapping)
        self.index = None
        self.has_changed = True

    def remove_unconnected(self):
        """removes nodes not part of the largest connected component
        """
        if self.base is None:
            raise NotImplementedError("unable to reorder unloaded base-object")
        self.base = _pyaccess_ext.remove_unconnected(self.base)
        self.index = None
        self.has_changed = True

def BaseObject_from_metadata(meta: dict[str, Any]) -> BaseObject:
    extent: Any = tuple(meta["extent"])
    obj = BaseObject(extent)
    return obj

def BaseObject_new(nodes: _pyaccess_ext.NodeVector, edges: _pyaccess_ext.EdgeVector) -> BaseObject:
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
    obj = BaseObject((minx, miny, maxx, maxy), base=base)
    return obj
