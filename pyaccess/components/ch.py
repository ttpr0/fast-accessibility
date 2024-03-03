from __future__ import annotations
from typing import Any
from enum import Enum, IntEnum
import os

from .. import _pyaccess_ext
from . import Ordering


class CHObject:
    base_weight: str
    base_partition: str | None
    id_ordering: Ordering

    ch_data: _pyaccess_ext.CHData | None
    ch_index: _pyaccess_ext.CHIndex | None
    id_mapping: _pyaccess_ext.IDMapping | None

    has_changed: bool

    def __init__(self, base_weight: str, ordering: Ordering, base_partition: str | None = None, ch_data: _pyaccess_ext.CHData | None = None, id_mapping: _pyaccess_ext.IDMapping | None = None):
        self.base_weight = base_weight
        self.id_ordering = ordering
        self.base_partition = base_partition

        self.ch_data = ch_data
        self.ch_index = None
        self.id_mapping = id_mapping

        if ch_data is None:
            self.has_changed = False
        else:
            self.has_changed = True

    def load(self, path: str):
        if not os.path.isfile(f"{path}-ch_data-ch_graph"):
            raise NotImplementedError("unable to find ch-object")
        if self.ch_data is None or self.has_changed == True:
            self.ch_data = _pyaccess_ext.load_ch_data(f"{path}-ch_data")
        if self.id_mapping is None or self.has_changed == True:
            self.id_mapping = _pyaccess_ext.load_id_mapping(f"{path}-id_mapping")

    def is_loaded(self) -> bool:
        if self.ch_data is None or self.id_mapping is None:
            return False
        return True

    def store(self, path: str):
        if self.ch_data is None or self.id_mapping is None:
            raise NotImplementedError("storing unloaded ch-object not possibile")
        if not self.has_changed:
            return
        _pyaccess_ext.store_ch_data(self.ch_data, f"{path}-ch_data")
        _pyaccess_ext.store_id_mapping(self.id_mapping, f"{path}-id_mapping")
        self.has_changed = False

    def delete(self, path: str):
        if os.path.isfile(f"{path}-ch_data-ch_graph"):
            os.remove(f"{path}-ch_data-ch_graph")
            os.remove(f"{path}-ch_data-shortcut")
            os.remove(f"{path}-ch_data-level")
        if os.path.isfile(f"{path}-id_mapping"):
            os.remove(f"{path}-id_mapping")
        self.ch_data = None
        self.ch_index = None
        self.id_mapping = None

    def get_base_weigth(self) -> str:
        return self.base_weight

    def get_base_partition(self) -> str | None:
        return self.base_partition

    def get_ch_data(self) -> _pyaccess_ext.CHData:
        if self.ch_data is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.ch_data

    def get_id_mapping(self) -> _pyaccess_ext.IDMapping:
        if self.id_mapping is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.id_mapping

    def has_ch_index(self) -> bool:
        return self.ch_index is not None

    def build_ch_index(self, base: _pyaccess_ext.GraphBase, weight: _pyaccess_ext.Weighting, partition: _pyaccess_ext.Partition | None = None):
        if self.ch_data is None or self.id_mapping is None:
            raise NotImplementedError("this should not have happened, please load first")
        if self.base_partition is not None:
            if partition is None:
                raise ValueError("partition is needed for building ch-index")
            if self.id_ordering not in [Ordering.TILE_LEVEL_ORDERING]:
                mapping = _pyaccess_ext.calc_ch2_order(base, partition, self.ch_data, self.id_mapping)
                self.reorder(Ordering.LEVEL_ORDERING, mapping)
            self.ch_index = _pyaccess_ext.prepare_ch_index_2(base, weight, self.ch_data, partition, self.id_mapping)
        else:
            if self.id_ordering not in [Ordering.LEVEL_ORDERING, Ordering.TILE_LEVEL_ORDERING]:
                mapping = _pyaccess_ext.calc_ch_order(base, self.ch_data, self.id_mapping)
                self.reorder(Ordering.LEVEL_ORDERING, mapping)
            self.ch_index = _pyaccess_ext.prepare_ch_index(base, weight, self.ch_data, self.id_mapping)

    def get_ch_index(self) -> _pyaccess_ext.CHIndex:
        if self.ch_index is None:
            raise NotImplementedError("this should not have happened, please build first")
        return self.ch_index

    def get_metadata(self) -> Any:
        meta = {
            "weight": self.base_weight,
            "ordering": int(self.id_ordering),
        }
        if self.base_partition is not None:
            meta["parition"] = self.base_partition
        return meta

    def reorder(self, ordering: Ordering, mapping: _pyaccess_ext.IntVector):
        """reorders nodes only for ch-object
        """
        if self.ch_data is None or self.id_mapping is None:
            raise NotImplementedError("unable to reorder unloaded ch-object")
        self.id_ordering = ordering
        self.ch_data = _pyaccess_ext.reorder_nodes(self.ch_data, mapping)
        if self.ch_index is not None:
            self.ch_index = _pyaccess_ext.reorder_nodes(self.ch_index, mapping)
        self.id_mapping = _pyaccess_ext.reorder_targets(self.id_mapping, mapping)
        self.has_changed = True

    def reorder_base(self, ordering: Ordering, mapping: _pyaccess_ext.IntVector, only_base: bool = True):
        """updates iternal id-mapping if base nodes are reordered

        Args:
            only_base: if True it does not update node-ordering inside of ch-object
        """
        if self.ch_data is None or self.id_mapping is None:
            raise NotImplementedError("unable to reorder unloaded ch-object")
        self.id_mapping = _pyaccess_ext.reorder_sources(self.id_mapping, mapping)
        if not only_base:
            self.reorder(ordering, mapping)
        self.has_changed = True

def CHObject_from_metadata(meta: dict[str, Any]) -> CHObject:
    weight = meta["weight"]
    ordering = Ordering(int(meta["ordering"]))
    if "partition" in meta:
        partition = meta["partition"]
    else:
        partition = None
    obj = CHObject(weight, ordering, base_partition=partition)
    return obj

def CHObject_new(base_weight: str, ch_data: _pyaccess_ext.CHData, id_mapping: _pyaccess_ext.IDMapping, base_partition: str | None = None) -> CHObject:
    obj = CHObject(base_weight, Ordering.BASE_ORDERING, base_partition=base_partition, ch_data=ch_data, id_mapping=id_mapping)
    return obj
