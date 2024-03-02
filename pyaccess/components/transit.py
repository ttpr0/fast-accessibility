from __future__ import annotations
from typing import Any
import os

from .. import _pyaccess_ext


class TransitObject:
    base_weight: str

    transit_data: _pyaccess_ext.TransitData | None
    id_mapping: _pyaccess_ext.IDMapping | None
    weightings: dict[str, _pyaccess_ext.TransitWeighting | None]

    has_changed: bool

    def __init__(self, base_weight: str, transit_data: _pyaccess_ext.TransitData | None = None, id_mapping: _pyaccess_ext.IDMapping | None = None, weightings: dict[str, _pyaccess_ext.TransitWeighting | None] = {}):
        self.base_weight = base_weight

        self.transit_data = transit_data
        self.id_mapping = id_mapping
        self.weightings = weightings

        if transit_data is None:
            self.has_changed = False
        else:
            self.has_changed = True

    def load(self, path: str):
        if not os.path.isfile(f"{path}-id_mapping"):
            raise NotImplementedError("unable to find transit-object")
        if self.transit_data is None or self.has_changed == True:
            self.transit_data = _pyaccess_ext.load_transit_data(f"{path}-transit_data")
        if self.id_mapping is None or self.has_changed == True:
            self.id_mapping = _pyaccess_ext.load_id_mapping(f"{path}-id_mapping")

    def is_loaded(self) -> bool:
        if self.transit_data is None or self.id_mapping is None:
            return False
        return True

    def load_weighting(self, name: str, path: str):
        if self.weightings[name] is None or self.has_changed == True:
            self.weightings[name] = _pyaccess_ext.load_transit_weights(f"{path}-weight-{name}")

    def is_weighting_loaded(self, name: str) -> bool:
        if name not in self.weightings:
            raise ValueError(f"weighting {name} does not exist")
        return self.weightings[name] != None

    def add_weighting(self, name: str, weighting: _pyaccess_ext.TransitWeighting):
        if name in self.weightings:
            raise ValueError(f"weighting {name} already exists")
        self.weightings[name] = weighting

    def delete_weighting(self, name: str, path: str):
        if name not in self.weightings:
            raise ValueError(f"weighting {name} does not exist")
        if os.path.isfile(f"{path}-weight-{name}"):
            os.remove(f"{path}-weight-{name}")
        del self.weightings[name]

    def store(self, path: str):
        if self.transit_data is None or self.id_mapping is None:
            raise NotImplementedError("storing unloaded transit-object not possibile")
        if not self.has_changed:
            return
        _pyaccess_ext.store_transit_data(self.transit_data, f"{path}-transit_data")
        _pyaccess_ext.store_id_mapping(self.id_mapping, f"{path}-id_mapping")
        for w in self.weightings:
            weights = self.weightings[w]
            if weights is None:
                continue
            _pyaccess_ext.store_transit_weights(weights, f"{path}-weight-{w}")
        self.has_changed = False

    def delete(self, path: str):
        if os.path.isfile(f"{path}-transit_data-comps"):
            os.remove(f"{path}-transit_data-comps")
            os.remove(f"{path}-transit_data-adjacency")
        if os.path.isfile(f"{path}-id_mapping"):
            os.remove(f"{path}-id_mapping")
        self.transit_data = None
        self.id_mapping = None
        for w in self.weightings:
            if os.path.isfile(f"{path}-weight-{w}"):
                os.remove(f"{path}-weight-{w}")
        self.weightings = {}

    def get_base_weigth(self) -> str:
        return self.base_weight

    def get_transit_data(self) -> _pyaccess_ext.TransitData:
        if self.transit_data is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.transit_data

    def get_id_mapping(self) -> _pyaccess_ext.IDMapping:
        if self.id_mapping is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.id_mapping
    
    def get_weightings(self) -> list[str]:
        return list(self.weightings.keys())
    
    def get_weighting(self, name: str) -> _pyaccess_ext.TransitWeighting:
        w = self.weightings[name]
        if w is None:
            raise NotImplementedError("this should not have happened, please load first")
        return w

    def get_metadata(self) -> Any:
        meta = {
            "weight": self.base_weight,
            "transit_weights": list(self.weightings.keys()),
        }
        return meta

    def reorder_base(self, mapping: _pyaccess_ext.IntVector):
        """updates internal id-mapping if base nodes are reordered
        """
        if self.transit_data is None or self.id_mapping is None:
            raise NotImplementedError("unable to reorder unloaded transit-object")
        self.id_mapping = _pyaccess_ext.reorder_sources(self.id_mapping, mapping)
        self.has_changed = True

def TransitObject_from_metadata(meta: dict[str, Any]) -> TransitObject:
    weight = meta["weight"]
    transit_weights = meta["transit_weights"]
    weightings = {}
    for w in transit_weights:
        weightings[w] = None
    obj = TransitObject(weight, weightings=weightings)
    return obj

def TransitObject_new(base_weight: str, transit_data: _pyaccess_ext.TransitData, id_mapping: _pyaccess_ext.IDMapping) -> TransitObject:
    obj = TransitObject(base_weight, transit_data=transit_data, id_mapping=id_mapping)
    return obj
