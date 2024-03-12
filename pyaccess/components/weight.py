from __future__ import annotations
from typing import Any
from enum import Enum, IntEnum
import os

from .. import _pyaccess_ext
from . import Ordering


class WeightObject:
    has_turn_costs: bool

    weight: _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting | None

    has_changed: bool

    def __init__(self, weight: _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting | None = None):
        if weight is None:
            self.has_changed = False
        else:
            self.has_changed = True

        if isinstance(weight, _pyaccess_ext.TCWeighting) and weight:
            self.has_turn_costs = True
        else:
            self.has_turn_costs = False
        self.weight = weight

    def load(self, path: str):
        if not os.path.isfile(f"{path}"):
            raise NotImplementedError("unable to find weight-object")
        if self.weight is None:
            if self.has_turn_costs:
                self.weight = _pyaccess_ext.load_tc_weights(f"{path}")
            else:
                self.weight = _pyaccess_ext.load_edge_weights(f"{path}")
            self.has_changed = False

    def is_loaded(self) -> bool:
        return self.weight is not None

    def store(self, path: str):
        if self.weight is None:
            raise NotImplementedError("storing unloaded weight-object not possibile")
        if not self.has_changed:
            return
        if isinstance(self.weight, _pyaccess_ext.TCWeighting):
            _pyaccess_ext.store_tc_weights(self.weight, f"{path}")
        else:
            _pyaccess_ext.store_edge_weights(self.weight, f"{path}")
        self.has_changed = False

    def delete(self, path: str):
        if os.path.isfile(f"{path}"):
            os.remove(f"{path}")
        self.weight = None

    def is_weighting(self) -> bool:
        return not self.has_turn_costs

    def is_tc_weighting(self) -> bool:
        return self.has_turn_costs

    def get_weight(self) -> _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting:
        if self.weight is None:
            raise NotImplementedError("this should not have happened, please load first")
        return self.weight

    def get_metadata(self) -> Any:
        return {
            "has_turn_costs": self.has_turn_costs
        }

    def reorder(self, ordering: Ordering, mapping: _pyaccess_ext.IntVector):
        """reorders nodes only for ch-object
        """
        if self.weight is None:
            raise NotImplementedError("unable to reorder unloaded weight-object")
        self.weight = _pyaccess_ext.reorder_nodes(self.weight, mapping)
        self.has_changed = True

def WeightObject_from_metadata(meta: dict[str, Any]) -> WeightObject:
    obj = WeightObject()
    obj.has_turn_costs = meta["has_turn_costs"]
    return obj

def WeightObject_new(weight: _pyaccess_ext.Weighting | _pyaccess_ext.TCWeighting) -> WeightObject:
    obj = WeightObject(weight=weight)
    return obj
