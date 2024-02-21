from . import _pyaccess_ext

def binary_decay(max_dist: int) -> _pyaccess_ext.BinaryDecay:
    """Creates a binary distance weighting with value 1 within and 0 outside the given distance threshold.
    """
    if max_dist <= 0:
        raise ValueError("invalid max distance")
    return _pyaccess_ext.BinaryDecay(max_dist)

def linear_decay(max_dist: int) -> _pyaccess_ext.LinearDecay:
    """Creates a linear distance weighting ranging from 1 (at range 0) to 0 (at the distance threshold).
    """
    if max_dist <= 0:
        raise ValueError("invalid max distance")
    return _pyaccess_ext.LinearDecay(max_dist)

def hybrid_decay(ranges: list[int], factors: list[float]) -> _pyaccess_ext.HybridDecay:
    """Creates a hybrid distance weighting from the given ranges and factors with multiple distance thresholds.

    Factors are applied to distances within the corresponding distance threshold.
    """
    if len(ranges) != len(factors):
        raise ValueError("invalid ranges and factors")
    combinded = list(zip(ranges, factors))
    combinded.sort(key=lambda x: x[0])
    if combinded[0][0] <= 0:
        raise ValueError("invalid ranges and factors")
    ranges = [x[0] for x in combinded]
    factors = [x[1] for x in combinded]
    return _pyaccess_ext.HybridDecay(_pyaccess_ext.IntVector(ranges), _pyaccess_ext.FloatVector(factors))
