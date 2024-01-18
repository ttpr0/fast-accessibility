from __future__ import annotations
from typing import Any
import os
import json

from . import _pyaccess_ext
from .components import Ordering
from .components.ch import CHObject, CHObject_from_metadata, CHObject_new
from .components.base import BaseObject, BaseObject_from_metadata, BaseObject_new
from .components.weight import WeightObject, WeightObject_from_metadata, WeightObject_new
from .components.partition import PartitionObject, PartitionObject_from_metadata, PartitionObject_new
from .components.tiled import TiledObject, TiledObject_new, TiledObject_from_metadata
from .explorer import Explorer

class Graph:
    _base_path: str
    _name: str

    base: BaseObject
    weights: dict[str, WeightObject]
    partitions: dict[str, PartitionObject]
    ch: dict[str, CHObject]
    tiled: dict[str, TiledObject]

    def __init__(self, _base_path: str, _name: str, base: BaseObject, weights: dict[str, WeightObject], partitions: dict[str, PartitionObject], ch: dict[str, CHObject], tiled: dict[str, TiledObject]):
        self._base_path = _base_path
        self._name = _name

        self.base = base

        self.weights = weights
        self.partitions = partitions
        self.ch = ch
        self.tiled = tiled

    def _get_path(self) -> str:
        return self._base_path

    def _get_name(self) -> str:
        return self._name

    def get_explorer(self, weight: str = "default") -> Explorer:
        return Explorer(self._get_base(), self._get_weight(weight))

    def store(self, name: str | None = None, path: str | None = None):
        if path is not None:
            self._base_path = path
        if name is not None:
            self._name = name
        meta = {}
        meta["base"] = self.base.get_metadata()
        self.base.store(f"{self._base_path}/{self._name}")
        meta["weights"] = {}
        for w, o in self.weights.items():
            meta["weights"][w] = o.get_metadata()
            o.store(f"{self._base_path}/{self._name}_{w}")
        meta["partitions"] = {}
        for p, o in self.partitions.items():
            meta["partitions"][p] = o.get_metadata()
            o.store(f"{self._base_path}/{self._name}_partition_{p}")
        meta["speed_ups"] = {}
        meta["speed_ups"]["ch"] = {}
        for c, o in self.ch.items():
            meta["speed_ups"]["ch"][c] = o.get_metadata()
            o.store(f"{self._base_path}/{self._name}_ch_{c}")
        meta["speed_ups"]["tiled"] = {}
        for t, o in self.tiled.items():
            meta["speed_ups"]["tiled"][t] = o.get_metadata()
            o.store(f"{self._base_path}/{self._name}_tiled_{t}")
        with open(f"{self._base_path}/{self._name}-meta", "w") as file:
            file.write(json.dumps(meta))

    def delete(self):
        if os.path.isfile(f"{self._base_path}/{self._name}-meta"):
            os.remove(f"{self._base_path}/{self._name}-meta")
        self.base.delete(f"{self._base_path}/{self._name}")
        for name, weight in self.weights.items():
            weight.delete(f"{self._base_path}/{self._name}_{name}")
        self.weights = {}
        for name, partition in self.partitions.items():
            partition.delete(f"{self._base_path}/{self._name}_partition_{name}")
        self.partitions = {}
        for name, ch in self.ch.items():
            ch.delete(f"{self._base_path}/{self._name}_ch_{name}")
        self.ch = {}
        for name, tiled in self.tiled.items():
            tiled.delete(f"{self._base_path}/{self._name}_tiled_{name}")
        self.tiled = {}

    def optimize_base(self):
        """removes all unconnected components and reorders graph with DFS-Ordering
        """
        if not self.base.is_loaded():
            self.base.load(f"{self._base_path}/{self._name}")
        self.base.remove_unconnected()
        base = self.base.get_base()
        mapping = _pyaccess_ext.calc_dfs_order(base)
        self.base.reorder(Ordering.DFS_ORDERING, mapping)
        # TODO: reorder or delete other components

    def add_default_weighting(self, name: str = "default"):
        if name in self.weights:
            raise ValueError(f"weighting {name} already exists")
        base = self._get_base()
        weight = _pyaccess_ext.prepare_default_weighting(base)
        weight_obj = WeightObject_new(weight)
        self.weights[name] = weight_obj

    def add_weighting(self, name: str, weights: _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting):
        if name in self.weights:
            raise ValueError(f"weighting {name} already exists")
        weight_obj = WeightObject_new(weights)
        self.weights[name] = weight_obj

    def add_partition(self, name: str, cell_count: int):
        if name in self.partitions:
            raise ValueError(f"partition {name} already exists")
        base = self._get_base()
        weight = _pyaccess_ext.prepare_default_weighting(base)
        partition = _pyaccess_ext.prepare_partition(base, weight, cell_count)
        partition_obj = PartitionObject_new(partition)
        self.partitions[name] = partition_obj

    def add_contraction(self, name: str, weight: str, partition: str | None = None):
        if name in self.ch:
            raise ValueError(f"contraction {name} already exists")
        b = self._get_base()
        w = self._get_weight(weight)
        if not isinstance(w, _pyaccess_ext.Weighting):
            raise ValueError("contraction can only be build for static weightings without turn costs")
        if partition is not None:
            p = self._get_partition(partition)
            ch = _pyaccess_ext.prepare_ch2(b, w, p)
        else:
            p = None
            ch = _pyaccess_ext.prepare_ch(b, w)
        id_mapping = _pyaccess_ext.new_id_mapping(b.node_count())
        ch_obj = CHObject_new(weight, ch, id_mapping, partition)
        self.ch[name] = ch_obj

    def add_grasp_overlay(self, name: str, weight: str, partition: str):
        if name in self.ch:
            raise ValueError(f"overlay {name} already exists")
        b = self._get_base()
        w = self._get_weight(weight)
        if not isinstance(w, _pyaccess_ext.Weighting):
            raise ValueError("grasp can only be build for static weightings without turn costs")
        p = self._get_partition(partition)
        tiled_data = _pyaccess_ext.prepare_tiled(b, w, p)
        cell_index = _pyaccess_ext.prepare_cell_index(b, w, p)
        id_mapping = _pyaccess_ext.new_id_mapping(b.node_count())
        tiled_obj = TiledObject_new(weight, partition, tiled_data, cell_index, id_mapping)
        self.tiled[name] = tiled_obj

    def add_isophast_overlay(self, name: str, weight: str, partition: str):
        if name in self.ch:
            raise ValueError(f"overlay {name} already exists")
        b = self._get_base()
        w = self._get_weight(weight)
        if not isinstance(w, _pyaccess_ext.Weighting):
            raise ValueError("isophast can only be build for static weightings without turn costs")
        p = self._get_partition(partition)
        tiled_data, cell_index = _pyaccess_ext.prepare_isophast(b, w, p)
        id_mapping = _pyaccess_ext.new_id_mapping(b.node_count())
        tiled_obj = TiledObject_new(weight, partition, tiled_data, cell_index, id_mapping)
        self.tiled[name] = tiled_obj

    def _get_base(self) -> _pyaccess_ext.GraphBase:
        if not self.base.is_loaded():
            self.base.load(f"{self._base_path}/{self._name}")
        return self.base.get_base()

    def _get_index(self) -> _pyaccess_ext.IGraphIndex:
        if not self.base.is_loaded():
            self.base.load(f"{self._base_path}/{self._name}")
        if not self.base.has_index():
            self.base.build_index()
        return self.base.get_index()

    def _get_weight(self, name: str) -> _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting:
        w = self.weights[name]
        if not w.is_loaded():
            w.load(f"{self._base_path}/{self._name}_{name}")
        weight = w.get_weight()
        return weight

    def _get_partition(self, name: str) -> _pyaccess_ext.Partition:
        p = self.partitions[name]
        if not p.is_loaded():
            p.load(f"{self._base_path}/{self._name}_partition_{name}")
        return p.get_partition()

    def _get_ch(self, name: str) -> tuple[_pyaccess_ext.CHData, _pyaccess_ext.CHIndex, _pyaccess_ext.IDMapping]:
        ch = self.ch[name]
        if not ch.is_loaded():
            ch.load(f"{self._base_path}/{self._name}_ch_{name}")
        if not ch.has_ch_index():
            base = self._get_base()
            w = ch.get_base_weigth()
            weight = self._get_weight(w)
            if not isinstance(weight, _pyaccess_ext.Weighting):
                raise ValueError("this should not have happened")
            p = ch.get_base_partition()
            if p is not None:
                partition = self._get_partition(p)
                ch.build_ch_index(base, weight, partition)
            else:
                ch.build_ch_index(base, weight)
        ch_data = ch.get_ch_data()
        id_mapping = ch.get_id_mapping()
        ch_index = ch.get_ch_index()
        return ch_data, ch_index, id_mapping

    def _get_ch_weight(self, name: str) -> str:
        ch = self.ch[name]
        return ch.get_base_weigth()

    def _get_ch_partition(self, name: str) -> str | None:
        ch = self.ch[name]
        return ch.get_base_partition()

    def _get_overlay(self, name: str) -> tuple[_pyaccess_ext.TiledData, _pyaccess_ext.CellIndex, _pyaccess_ext.IDMapping]:
        tiled = self.tiled[name]
        if not tiled.is_loaded():
            tiled.load(f"{self._base_path}/{self._name}_tiled_{name}")
        tiled_data = tiled.get_tiled_data()
        id_mapping = tiled.get_id_mapping()
        cell_index = tiled.get_cell_index()
        return tiled_data, cell_index, id_mapping

    def _get_overlay_weight(self, name: str) -> str:
        tiled = self.tiled[name]
        return tiled.get_base_weigth()

    def _get_overlay_partition(self, name: str) -> str:
        tiled = self.tiled[name]
        return tiled.get_base_partition()

def new_graph(nodes: _pyaccess_ext.NodeVector, edges: _pyaccess_ext.EdgeVector) -> Graph:
    base = BaseObject_new(nodes, edges)
    return Graph("", "", base, {}, {}, {}, {})

def load_graph(name: str, path: str) -> Graph:
    meta: dict
    with open(f"{path}/{name}-meta", "r") as file:
        meta = json.loads(file.read())
    base = BaseObject_from_metadata(meta["base"])
    weights = {}
    for w, m in meta["weights"].items():
        weights[w] = WeightObject_from_metadata(m)
    partitions = {}
    for p, m in meta["partitions"].items():
        partitions[p] = PartitionObject_from_metadata(m)
    ch = {}
    for c, m in meta["speed_ups"]["ch"].items():
        ch[c] = CHObject_from_metadata(m)
    tiled = {}
    for t, m in meta["speed_ups"]["tiled"].items():
        tiled[t] = TiledObject_from_metadata(m)
    return Graph(path, name, base, weights, partitions, ch, tiled)
