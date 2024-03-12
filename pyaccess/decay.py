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

def exponential_decay(max_dist: int) -> _pyaccess_ext.ExponentialDecay:
    """Creates an exponential distance weighting ranging from 1 (at range 0) to 0 (at the distance threshold).
    """
    if max_dist <= 0:
        raise ValueError("invalid max distance")
    return _pyaccess_ext.ExponentialDecay(max_dist)

def gaussian_decay(max_dist: int) -> _pyaccess_ext.GaussianDecay:
    """Creates a gaussian distance weighting ranging from 1 (at range 0) to 0 (at the distance threshold).
    """
    if max_dist <= 0:
        raise ValueError("invalid max distance")
    return _pyaccess_ext.GaussianDecay(max_dist)

def gravity_decay(max_dist: int, beta: float) -> _pyaccess_ext.GravityDecay:
    """Creates a gravity distance weighting ranging from 1 (at range 0) to 0 (at the distance threshold).
    """
    if max_dist <= 0:
        raise ValueError("invalid max distance")
    return _pyaccess_ext.GravityDecay(max_dist, beta)

def inverse_power_decay(max_dist: int) -> _pyaccess_ext.InversePowerDecay:
    """Creates an inverse-power distance weighting ranging from 1 (at range 0) to 0 (at the distance threshold).
    """
    if max_dist <= 0:
        raise ValueError("invalid max distance")
    return _pyaccess_ext.InversePowerDecay(max_dist)

def kernel_density_decay(max_dist: int, factor: float) -> _pyaccess_ext.KernelDensityDecay:
    """Creates a kernel-density distance weighting ranging from 1 (at range 0) to 0 (at the distance threshold).
    """
    if max_dist <= 0:
        raise ValueError("invalid max distance")
    return _pyaccess_ext.KernelDensityDecay(max_dist, factor)

def polynomial_decay(max_dist: int, koefficients: list[float]) -> _pyaccess_ext.PolynomDecay:
    """Creates a polynomial distance weighting using the provided koefficients.
    """
    if max_dist <= 0:
        raise ValueError("invalid max distance")
    return _pyaccess_ext.PolynomDecay(max_dist, _pyaccess_ext.FloatVector(koefficients))
