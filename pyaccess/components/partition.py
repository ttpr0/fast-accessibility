from __future__ import annotations
from typing import Any
import os

from .. import _pyaccess_ext
from . import Ordering


class PartitionObject:

    partition: _pyaccess_ext.Partition | None

    has_changed: bool

    def __init__(self, partition: _pyaccess_ext.Partition | None = None):
        if partition is None:
            self.has_changed = False
        else:
            self.has_changed = True

        self.partition = partition

    def load(self, path: str):
        if not os.path.isfile(f"{path}"):
            raise NotImplementedError("unable to find partition-object")
        if self.partition is None:
            self.partition = _pyaccess_ext.load_node_partition(f"{path}")
            self.has_changed = False

    def is_loaded(self) -> bool:
        return self.partition is not None

    def store(self, path: str):
        if self.partition is None:
            raise NotImplementedError("storing unloaded partition-object not possibile")
        if not self.has_changed:
            return
        _pyaccess_ext.store_node_partition(self.partition, f"{path}")
        self.has_changed = False

    def delete(self, path: str):
        if not os.path.isfile(f"{path}"):
            os.remove(f"{path}")
        self.weight = None

    def get_partition(self) -> _pyaccess_ext.Partition:
        if self.partition is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.partition

    def get_metadata(self) -> Any:
        return {
        }

    def reorder(self, ordering: Ordering, mapping: _pyaccess_ext.IntVector):
        """reorders nodes only for ch-object
        """
        if self.partition is None:
            raise NotImplementedError("unable to reorder unloaded partition-object")
        self.partition = _pyaccess_ext.reorder_nodes(self.partition, mapping)
        self.has_changed = True

def PartitionObject_from_metadata(meta: dict[str, Any]) -> PartitionObject:
    obj = PartitionObject()
    return obj

def PartitionObject_new(partition: _pyaccess_ext.Partition) -> PartitionObject:
    obj = PartitionObject(partition=partition)
    return obj
