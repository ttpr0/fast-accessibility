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

        self.has_changed = False

    def load(self, path: str):
        # TODO
        ...

    def is_loaded(self) -> bool:
        if self.transit_data is None or self.id_mapping is None:
            return False
        return True

    def load_weighting(self, path: str, name: str):
        # TODO
        ...

    def is_weighting_loaded(self, name: str) -> bool:
        if name not in self.weightings:
            raise ValueError(f"weighting {name} does not exist")
        return self.weightings[name] != None

    def add_weighting(self, name: str, weighting: _pyaccess_ext.TransitWeighting):
        if name in self.weightings:
            raise ValueError(f"weighting {name} already exists")
        self.weightings[name] = weighting

    def delete_weighting(self, name: str):
        if name not in self.weightings:
            raise ValueError(f"weighting {name} does not exist")
        del self.weightings[name]
        # TODO

    def store(self, path: str):
        # TODO
        ...

    def delete(self, path: str):
        # TODO
        ...

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
