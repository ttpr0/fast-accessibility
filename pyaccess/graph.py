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
from .components.transit import TransitObject, TransitObject_new, TransitObject_from_metadata
from .explorer import Explorer

class Graph:
    _base_path: str | None
    _name: str | None

    _base: BaseObject
    _weights: dict[str, WeightObject]
    _partitions: dict[str, PartitionObject]
    _ch: dict[str, CHObject]
    _tiled: dict[str, TiledObject]
    _transit: dict[str, TransitObject]

    def __init__(self, _base_path: str | None, _name: str | None, base: BaseObject, weights: dict[str, WeightObject], partitions: dict[str, PartitionObject], ch: dict[str, CHObject], tiled: dict[str, TiledObject], transit: dict[str, TransitObject]):
        self._base_path = _base_path
        self._name = _name

        self._base = base

        self._weights = weights
        self._partitions = partitions
        self._ch = ch
        self._tiled = tiled
        self._transit = transit

    def _get_path(self) -> str | None:
        return self._base_path

    def _get_name(self) -> str | None:
        return self._name

    def get_explorer(self, weight: str | None = None, partition: str | None = None, ch: str | None = None, overlay: str | None = None, transit: str | None = None, transit_weight: str | None = None) -> Explorer:
        """Creates a graph-explorer to traverse the graphs nodes, edges and shortcuts.
        """
        b = self._get_base()
        i = self._get_index()
        w = None
        p = None
        im = None
        c = None
        o = None
        t = None
        sm = None
        tw = None
        if weight is not None:
            w = self._get_weight(weight)
        if partition is not None:
            p = self._get_partition(partition)
        if ch is not None:
            c, _, im = self._get_ch(ch)
            w = self._get_weight(self._get_ch_weight(ch))
            part = self._get_ch_partition(ch)
            if part is not None:
                p = self._get_partition(part)
        elif overlay is not None:
            o, _, im = self._get_overlay(overlay)
            w = self._get_weight(self._get_overlay_weight(overlay))
            p = self._get_partition(self._get_overlay_partition(overlay))
        elif transit is not None:
            t, sm = self._get_transit(transit)
            w = self._get_weight(self._get_transit_base_weight(transit))
            if transit_weight is not None:
                tw = self._get_transit_weighting(transit, transit_weight)
        return Explorer(b, i, w, p, im, c, o, t, sm, tw)

    def store(self, name: str | None = None, path: str | None = None):
        """Stores the graph into the given path.
        """
        if path is not None:
            self._base_path = path
        if name is not None:
            self._name = name
        if self._base_path is None or self._name is None:
            raise ValueError("No path or name given.")
        meta = {}
        meta["base"] = self._base.get_metadata()
        self._base.store(f"{self._base_path}/{self._name}")
        meta["weights"] = {}
        for w, o in self._weights.items():
            meta["weights"][w] = o.get_metadata()
            o.store(f"{self._base_path}/{self._name}_{w}")
        meta["partitions"] = {}
        for p, o in self._partitions.items():
            meta["partitions"][p] = o.get_metadata()
            o.store(f"{self._base_path}/{self._name}_partition_{p}")
        meta["speed_ups"] = {}
        meta["speed_ups"]["ch"] = {}
        for c, o in self._ch.items():
            meta["speed_ups"]["ch"][c] = o.get_metadata()
            o.store(f"{self._base_path}/{self._name}_ch_{c}")
        meta["speed_ups"]["tiled"] = {}
        for t, o in self._tiled.items():
            meta["speed_ups"]["tiled"][t] = o.get_metadata()
            o.store(f"{self._base_path}/{self._name}_tiled_{t}")
        meta["transit"] = {}
        for t, o in self._transit.items():
            meta["transit"][t] = o.get_metadata()
            o.store(f"{self._base_path}/{self._name}_transit_{t}")
        with open(f"{self._base_path}/{self._name}-meta", "w") as file:
            file.write(json.dumps(meta))

    def delete(self):
        """Deletes the graph and all components (removing everything from disk).
        """
        if self._base_path is None or self._name is None:
            raise ValueError("Can't delete graph with unknown name and path.")
        if os.path.isfile(f"{self._base_path}/{self._name}-meta"):
            os.remove(f"{self._base_path}/{self._name}-meta")
        self._base.delete(f"{self._base_path}/{self._name}")
        for name, weight in self._weights.items():
            weight.delete(f"{self._base_path}/{self._name}_{name}")
        self._weights = {}
        for name, partition in self._partitions.items():
            partition.delete(f"{self._base_path}/{self._name}_partition_{name}")
        self._partitions = {}
        for name, ch in self._ch.items():
            ch.delete(f"{self._base_path}/{self._name}_ch_{name}")
        self._ch = {}
        for name, tiled in self._tiled.items():
            tiled.delete(f"{self._base_path}/{self._name}_tiled_{name}")
        self._tiled = {}
        for name, transit in self._transit.items():
            transit.delete(f"{self._base_path}/{self._name}_transit_{name}")
        self._transit = {}

    def optimize_base(self):
        """removes all unconnected components and reorders graph with DFS-Ordering
        """
        change_stored = self._base_path is None or self._name is None
        if not self._base.is_loaded():
            if change_stored:
                self._base.load(f"{self._base_path}/{self._name}")
        self._base.remove_unconnected()
        base = self._base.get_base()
        mapping = _pyaccess_ext.calc_dfs_order(base)
        self._base.reorder(Ordering.DFS_ORDERING, mapping)
        if change_stored:
            for w, o in self._weights.items():
                o.delete(f"{self._base_path}/{self._name}_{w}")
        self._weights = {}
        if change_stored:
            for p, o in self._partitions.items():
                o.delete(f"{self._base_path}/{self._name}_partition_{p}")
        self._partitions = {}
        if change_stored:
            for c, o in self._ch.items():
                o.delete(f"{self._base_path}/{self._name}_ch_{c}")
        self._ch = {}
        if change_stored:
            for t, o in self._tiled.items():
                o.delete(f"{self._base_path}/{self._name}_tiled_{t}")
        self._tiled = {}
        if change_stored:
            for name, transit in self._transit.items():
                transit.delete(f"{self._base_path}/{self._name}_transit_{name}")
        self._transit = {}

    def add_default_weighting(self, name: str = "default"):
        """Adds a new default weighting to the graph.

        Weights are the time cost of traversing a street (computed from edges maxspeed and roadtype).
        """
        if name in self._weights:
            raise ValueError(f"weighting {name} already exists")
        base = self._get_base()
        weight = _pyaccess_ext.prepare_default_weighting(base)
        weight_obj = WeightObject_new(weight)
        self._weights[name] = weight_obj

    def add_weighting(self, name: str, weights: _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting):
        """Adds a new weighting to the graph.

        To create a weighting use new_weighting(...), new_tc_weighting(...), etc.
        """
        if name in self._weights:
            raise ValueError(f"weighting {name} already exists")
        weight_obj = WeightObject_new(weights)
        self._weights[name] = weight_obj

    def add_partition(self, name: str, cell_count: int):
        """Computes a node-partition of graph using inertial-flow algorithm.
        """
        if name in self._partitions:
            raise ValueError(f"partition {name} already exists")
        base = self._get_base()
        weight = _pyaccess_ext.prepare_default_weighting(base)
        partition = _pyaccess_ext.prepare_partition(base, weight, cell_count)
        partition_obj = PartitionObject_new(partition)
        self._partitions[name] = partition_obj

    def add_contraction(self, name: str, weight: str, partition: str | None = None):
        """Contracts graph with given weighting using 2*ED + CN + EC + 5*L contraction order.

        If partition is specified cells will be contracted individually (similar to isophast).
        """
        if name in self._ch:
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
        self._ch[name] = ch_obj

    def add_grasp_overlay(self, name: str, weight: str, partition: str):
        """Computes grasp overlay from the given weighting and partition.
        """
        if name in self._tiled:
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
        self._tiled[name] = tiled_obj

    def add_isophast_overlay(self, name: str, weight: str, partition: str):
        """Computes isophast overlay from the given weighting and partition.
        """
        if name in self._ch:
            raise ValueError(f"overlay {name} already exists")
        b = self._get_base()
        w = self._get_weight(weight)
        if not isinstance(w, _pyaccess_ext.Weighting):
            raise ValueError("isophast can only be build for static weightings without turn costs")
        p = self._get_partition(partition)
        tiled_data, cell_index = _pyaccess_ext.prepare_isophast(b, w, p)
        id_mapping = _pyaccess_ext.new_id_mapping(b.node_count())
        tiled_obj = TiledObject_new(weight, partition, tiled_data, cell_index, id_mapping)
        self._tiled[name] = tiled_obj

    def add_public_transit(self, name: str, stops: _pyaccess_ext.NodeVector, connections: _pyaccess_ext.ConnectionVector, weight: str = "default", max_transfer_range: int = 900):
        """Adds public transit overlay.
        """
        b = self._get_base()
        w = self._get_weight(weight)
        i = self._get_index()
        if isinstance(w, _pyaccess_ext.Weighting):
            g = _pyaccess_ext.build_base_graph(b, w, i)
        else:
            g = _pyaccess_ext.build_tc_graph(b, w, i)
        transit_data, id_mapping = _pyaccess_ext.prepare_transit(g, stops, connections, max_transfer_range)
        transit_obj = TransitObject_new(weight, transit_data, id_mapping)
        self._transit[name] = transit_obj

    def add_transit_weighting(self, name: str, weights: _pyaccess_ext.TransitWeighting, transit: str):
        """Adds a transit weighting.
        """
        if transit not in self._transit:
            raise ValueError(f"transit {transit} does not exist")
        t = self._transit[transit]
        t.add_weighting(name, weights)

    def remove_weighting(self, name: str):
        """Removes and deletes a weighting and all dependant contractions and overlays.
        """
        if name not in self._weights:
            raise ValueError(f"weighting {name} does not exist")
        o = self._weights[name]
        if self._base_path is not None and self._name is not None:
            o.delete(f"{self._base_path}/{self._name}_{name}")
        del self._weights[name]
        rm_ch = []
        for c, o in self._ch.items():
            w = o.get_base_weigth()
            if w == name:
                rm_ch.append(c)
        for c in rm_ch:
            self.remove_ch(c)
        rm_ov = []
        for t, o in self._tiled.items():
            w = o.get_base_weigth()
            if w == name:
                rm_ov.append(t)
        for t in rm_ov:
            self.remove_overlay(t)

    def remove_partition(self, name: str):
        """Removes and deletes a partition and all dependant contractions and overlays.
        """
        if name not in self._partitions:
            raise ValueError(f"partition {name} does not exist")
        o = self._partitions[name]
        if self._base_path is not None and self._name is not None:
            o.delete(f"{self._base_path}/{self._name}_partition_{name}")
        del self._partitions[name]
        rm_ch = []
        for c, o in self._ch.items():
            p = o.get_base_partition()
            if p == name:
                rm_ch.append(c)
        for c in rm_ch:
            self.remove_ch(c)
        rm_ov = []
        for t, o in self._tiled.items():
            p = o.get_base_partition()
            if p == name:
                rm_ov.append(t)
        for t in rm_ov:
            self.remove_overlay(t)

    def remove_ch(self, name: str):
        """Removes and deletes a contraction.
        """
        if name not in self._ch:
            raise ValueError(f"ch {name} does not exist")
        o = self._ch[name]
        if self._base_path is not None and self._name is not None:
            o.delete(f"{self._base_path}/{self._name}_ch_{name}")
        del self._ch[name]

    def remove_overlay(self, name: str):
        """Removes and deletes an overlay.
        """
        if name not in self._tiled:
            raise ValueError(f"overlay {name} does not exist")
        o = self._tiled[name]
        if self._base_path is not None and self._name is not None:
            o.delete(f"{self._base_path}/{self._name}_tiled_{name}")
        del self._tiled[name]

    def remove_public_transit(self, name: str):
        """Removes and deletes an public-transit-overlay.
        """
        if name not in self._transit:
            raise ValueError(f"transit-overlay {name} does not exist")
        o = self._transit[name]
        if self._base_path is not None and self._name is not None:
            o.delete(f"{self._base_path}/{self._name}_transit_{name}")
        del self._transit[name]

    def remove_transit_weighting(self, name: str, transit: str):
        """Removes and deletes a transit weighting.
        """
        if transit not in self._transit:
            raise ValueError(f"transit {transit} does not exist")
        t = self._transit[transit]
        t.remove_weighting(name)

    def _get_base(self) -> _pyaccess_ext.GraphBase:
        if not self._base.is_loaded():
            if self._base_path is None or self._name is None:
                raise ValueError("Can't load base with unknown name and path.")
            self._base.load(f"{self._base_path}/{self._name}")
        return self._base.get_base()

    def _get_index(self) -> _pyaccess_ext.IGraphIndex:
        if not self._base.is_loaded():
            if self._base_path is None or self._name is None:
                raise ValueError("Can't load index with unknown name and path.")
            self._base.load(f"{self._base_path}/{self._name}")
        if not self._base.has_index():
            self._base.build_index()
        return self._base.get_index()

    def _get_weight(self, name: str) -> _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting:
        w = self._weights[name]
        if not w.is_loaded():
            if self._base_path is None or self._name is None:
                raise ValueError("Can't load weight with unknown name and path.")
            w.load(f"{self._base_path}/{self._name}_{name}")
        weight = w.get_weight()
        return weight

    def _get_partition(self, name: str) -> _pyaccess_ext.Partition:
        p = self._partitions[name]
        if not p.is_loaded():
            if self._base_path is None or self._name is None:
                raise ValueError("Can't load partition with unknown name and path.")
            p.load(f"{self._base_path}/{self._name}_partition_{name}")
        return p.get_partition()

    def _get_ch(self, name: str) -> tuple[_pyaccess_ext.CHData, _pyaccess_ext.CHIndex, _pyaccess_ext.IDMapping]:
        ch = self._ch[name]
        if not ch.is_loaded():
            if self._base_path is None or self._name is None:
                raise ValueError("Can't load contraction with unknown name and path.")
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
        ch = self._ch[name]
        return ch.get_base_weigth()

    def _get_ch_partition(self, name: str) -> str | None:
        ch = self._ch[name]
        return ch.get_base_partition()

    def _get_overlay(self, name: str) -> tuple[_pyaccess_ext.TiledData, _pyaccess_ext.CellIndex, _pyaccess_ext.IDMapping]:
        tiled = self._tiled[name]
        if not tiled.is_loaded():
            if self._base_path is None or self._name is None:
                raise ValueError("Can't load overlay with unknown name and path.")
            tiled.load(f"{self._base_path}/{self._name}_tiled_{name}")
        tiled_data = tiled.get_tiled_data()
        id_mapping = tiled.get_id_mapping()
        cell_index = tiled.get_cell_index()
        return tiled_data, cell_index, id_mapping

    def _get_overlay_weight(self, name: str) -> str:
        tiled = self._tiled[name]
        return tiled.get_base_weigth()

    def _get_overlay_partition(self, name: str) -> str:
        tiled = self._tiled[name]
        return tiled.get_base_partition()
    
    def _get_transit(self, name: str) -> tuple[_pyaccess_ext.TransitData, _pyaccess_ext.IDMapping]:
        transit = self._transit[name]
        if not transit.is_loaded():
            if self._base_path is None or self._name is None:
                raise ValueError("Can't load public transit with unknown name and path.")
            transit.load(f"{self._base_path}/{self._name}_transit_{name}")
        transit_data = transit.get_transit_data()
        id_mapping = transit.get_id_mapping()
        return transit_data, id_mapping
    
    def _get_transit_base_weight(self, name: str) -> str:
        transit = self._transit[name]
        return transit.get_base_weigth()

    def _get_transit_weighting(self, name: str, weighting: str) -> _pyaccess_ext.TransitWeighting:
        transit = self._transit[name]
        if not transit.is_loaded():
            if self._base_path is None or self._name is None:
                raise ValueError("Can't load public transit with unknown name and path.")
            transit.load(f"{self._base_path}/{self._name}_transit_{name}")
        return transit.get_weighting(weighting)


def new_graph(nodes: _pyaccess_ext.NodeVector, edges: _pyaccess_ext.EdgeVector) -> Graph:
    """Creates a new graph from nodes and edges.

    Use graph.store(...) to store and load_graph() to load a graph from a directory.
    """
    base = BaseObject_new(nodes, edges)
    return Graph(None, None, base, {}, {}, {}, {}, {})

def load_graph(name: str, path: str) -> Graph:
    """Loads graph from a directory.

    Components are lazily loaded thus first accessibility calculations will be slower.
    """
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
    transit = {}
    for t, m in meta["transit"].items():
        transit[t] = TransitObject_from_metadata(m)
    return Graph(path, name, base, weights, partitions, ch, tiled, transit)
