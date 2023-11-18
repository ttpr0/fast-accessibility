#pragma once

#include <iostream>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unique_ptr.h>

#include "../accessibility/distance_decay/binary_decay.h"
#include "../accessibility/distance_decay/decay.h"
#include "../accessibility/distance_decay/hybrid_decay.h"
#include "../accessibility/distance_decay/linear_decay.h"

void bind_decay(nanobind::module_& m)
{
    namespace py = nanobind;
    //*******************************************
    // decay classes
    //*******************************************

    auto i_decay = py::class_<IDistanceDecay>(m, "IDistanceDecay");
    i_decay.def("get_distance_weight", &IDistanceDecay::get_distance_weight, py::arg("distance"), "Computes the decayed weight (range [0, 1]) for the given distance.");
    i_decay.def("get_max_distance", &IDistanceDecay::get_max_distance, "Maximum distance to give distance-weight greater than 0.");

    auto binary_decay = py::class_<BinaryDecay, IDistanceDecay>(m, "BinaryDecay");
    binary_decay.def(py::init<int>(), py::arg("max_dist"));

    auto hybrid_decay = py::class_<HybridDecay, IDistanceDecay>(m, "HybridDecay");
    hybrid_decay.def(py::init<std::vector<int>, std::vector<float>>(), py::arg("distances"), py::arg("factors"));

    auto linear_decay = py::class_<LinearDecay, IDistanceDecay>(m, "LinearDecay");
    linear_decay.def(py::init<int>(), py::arg("max_dist"));
}
