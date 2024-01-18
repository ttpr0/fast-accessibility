from __future__ import annotations
from typing import Any
from enum import Enum, IntEnum
import os

from .. import _pyaccess_ext
from . import Ordering


class TiledObject:
    base_weight: str
    base_partition: str
    id_ordering: Ordering

    tiled_data: _pyaccess_ext.TiledData | None
    cell_index: _pyaccess_ext.CellIndex | None
    id_mapping: _pyaccess_ext.IDMapping | None

    has_changed: bool

    def __init__(self, base_weight: str, ordering: Ordering, base_partition: str, tiled_data: _pyaccess_ext.TiledData | None = None, cell_index: _pyaccess_ext.CellIndex | None = None, id_mapping: _pyaccess_ext.IDMapping | None = None):
        self.base_weight = base_weight
        self.id_ordering = ordering
        self.base_partition = base_partition

        self.tiled_data = tiled_data
        self.cell_index = cell_index
        self.id_mapping = id_mapping

        self.has_changed = False

    def load(self, path: str):
        if not os.path.isfile(f"{path}-ch_data"):
            raise NotImplementedError("unable to find ch-object")
        if self.tiled_data is None or self.has_changed == True:
            self.tiled_data = _pyaccess_ext.load_tiled_data(f"{path}-tiled_data", 0) # TODO: Remove need for node_count
        if self.cell_index is None or self.has_changed == True:
            self.cell_index = _pyaccess_ext.load_cell_index(f"{path}-cell_index")
        if self.id_mapping is None or self.has_changed == True:
            self.id_mapping = _pyaccess_ext.load_id_mapping(f"{path}-id_mapping")

    def is_loaded(self) -> bool:
        if self.tiled_data is None or self.cell_index is None or self.id_mapping is None:
            return False
        return True

    def store(self, path: str):
        if self.tiled_data is None or self.cell_index is None or self.id_mapping is None:
            raise NotImplementedError("storing unloaded ch-object not possibile")
        if not self.has_changed:
            return
        _pyaccess_ext.store_tiled_data(self.tiled_data, f"{path}-tiled_data", 0) # TODO: Remove need for node_count
        _pyaccess_ext.store_cell_index(self.cell_index, f"{path}-cell_index")
        _pyaccess_ext.store_id_mapping(self.id_mapping, f"{path}-id_mapping")
        self.has_changed = False

    def delete(self, path: str):
        if not os.path.isfile(f"{path}-tiled_data"):
            os.remove(f"{path}-tiled_data")
        if not os.path.isfile(f"{path}-cell_index"):
            os.remove(f"{path}-cell_index")
        if not os.path.isfile(f"{path}-id_mapping"):
            os.remove(f"{path}-id_mapping")
        self.ch_data = None
        self.ch_index = None
        self.id_mapping = None

    def get_base_weigth(self) -> str:
        return self.base_weight

    def get_base_partition(self) -> str:
        return self.base_partition

    def get_tiled_data(self) -> _pyaccess_ext.TiledData:
        if self.tiled_data is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.tiled_data

    def get_id_mapping(self) -> _pyaccess_ext.IDMapping:
        if self.id_mapping is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.id_mapping

    def get_cell_index(self) -> _pyaccess_ext.CellIndex:
        if self.cell_index is None:
            raise NotImplementedError("this should not have happened, please build first")
        return self.cell_index

    def get_metadata(self) -> Any:
        meta = {
            "weight": self.base_weight,
            "ordering": int(self.id_ordering),
            "partition": self.base_partition
        }
        return meta

    def reorder(self, ordering: Ordering, mapping: _pyaccess_ext.IntVector):
        """reorders nodes only for ch-object
        """
        if self.tiled_data is None or self.cell_index is None or self.id_mapping is None:
            raise NotImplementedError("unable to reorder unloaded ch-object")
        self.id_ordering = ordering
        self.tiled_data = _pyaccess_ext.reorder_nodes(self.tiled_data, mapping)
        if self.cell_index is not None:
            self.cell_index = _pyaccess_ext.reorder_nodes(self.cell_index, mapping)
        self.id_mapping = _pyaccess_ext.reorder_targets(self.id_mapping, mapping)
        self.has_changed = True

    def reorder_base(self, ordering: Ordering, mapping: _pyaccess_ext.IntVector, only_base: bool = True):
        """updates iternal id-mapping if base nodes are reordered

        Args:
            only_base: if True it does not update node-ordering inside of ch-object
        """
        if self.tiled_data is None or self.cell_index is None or self.id_mapping is None:
            raise NotImplementedError("unable to reorder unloaded ch-object")
        self.id_mapping = _pyaccess_ext.reorder_sources(self.id_mapping, mapping)
        if not only_base:
            self.reorder(ordering, mapping)
        self.has_changed = True

def TiledObject_from_metadata(meta: dict[str, Any]) -> TiledObject:
    weight = meta["weight"]
    ordering = Ordering(int(meta["ordering"]))
    partition = meta["partition"]
    obj = TiledObject(weight, ordering, partition)
    return obj

def TiledObject_new(base_weight: str, base_partition: str, tiled_data: _pyaccess_ext.TiledData, cell_index: _pyaccess_ext.CellIndex, id_mapping: _pyaccess_ext.IDMapping) -> TiledObject:
    obj = TiledObject(base_weight, Ordering.BASE_ORDERING, base_partition, tiled_data=tiled_data, cell_index=cell_index, id_mapping=id_mapping)
    return obj
