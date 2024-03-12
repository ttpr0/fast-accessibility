#pragma once

#include <iostream>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unique_ptr.h>

#include "accessibility/2sfca.h"
#include "accessibility/range_query.h"
#include "accessibility/reachability.h"
#include "graph/graph.h"
#include "solver/one_to_many/dijkstra.h"
#include "solver/one_to_many/grasp.h"
#include "solver/one_to_many/phast.h"
#include "solver/one_to_many/rphast.h"
#include "solver/one_to_many/transit.h"

void bind_solver(nanobind::module_& m)
{
    namespace py = nanobind;

    //*******************************************
    // one-to-many solvers
    //*******************************************

    auto range_dijkstra = py::class_<RangeDijkstra>(m, "RangeDijkstra");
    range_dijkstra.def("is_build", &RangeDijkstra::isBuild);
    m.def(
        "build_range_dijkstra_solver", [](IGraph* graph) { return RangeDijkstra(graph); }, py::keep_alive<1, 0>());

    auto range_phast = py::class_<RangePHAST>(m, "RangePHAST");
    range_phast.def("is_build", &RangePHAST::isBuild);
    m.def(
        "build_range_phast_solver", [](ICHGraph* graph) { return RangePHAST(graph); }, py::keep_alive<1, 0>());

    auto range_rphast = py::class_<RangeRPHAST>(m, "RangeRPHAST");
    range_rphast.def("is_build", &RangeRPHAST::isBuild);
    m.def(
        "build_range_rphast_solver", [](ICHGraph* graph) { return RangeRPHAST(graph); }, py::keep_alive<1, 0>());

    auto range_rphast2 = py::class_<RangeRPHAST2>(m, "RangeRPHAST2");
    range_rphast2.def("is_build", &RangeRPHAST2::isBuild);
    m.def(
        "build_range_rphast2_solver", [](ICHGraph* graph) { return RangeRPHAST2(graph); }, py::keep_alive<1, 0>());

    auto range_phast_gs = py::class_<RangePHASTGS>(m, "RangePHASTGS");
    range_phast_gs.def("is_build", &RangePHASTGS::isBuild);
    m.def(
        "build_range_phast_gs_solver", [](CHGraph2* graph) { return RangePHASTGS(graph); }, py::keep_alive<1, 0>());

    auto range_rphast_gs = py::class_<RangeRPHASTGS>(m, "RangeRPHASTGS");
    range_rphast_gs.def("is_build", &RangeRPHASTGS::isBuild);
    m.def(
        "build_range_rphast_gs_solver", [](CHGraph2* graph) { return RangeRPHASTGS(graph); }, py::keep_alive<1, 0>());

    auto grasp = py::class_<GRASP>(m, "GRASP");
    grasp.def("is_build", &GRASP::isBuild);
    m.def(
        "build_grasp_solver", [](ITiledGraph* graph) { return GRASP(graph); }, py::keep_alive<1, 0>());

    auto range_dijkstra_tc = py::class_<RangeDijkstraTC>(m, "RangeDijkstraTC");
    range_dijkstra_tc.def("is_build", &RangeDijkstraTC::isBuild);
    m.def(
        "build_range_dijkstra_tc_solver", [](IGraph* graph) { return RangeDijkstraTC(graph); }, py::keep_alive<1, 0>());

    auto transit_dijkstra = py::class_<TransitDijkstra>(m, "TransitDijkstra");
    transit_dijkstra.def("is_build", &TransitDijkstra::isBuild);
    transit_dijkstra.def("set_min_departure", &TransitDijkstra::setMinDeparture);
    transit_dijkstra.def("set_max_departure", &TransitDijkstra::setMaxDeparture);
    m.def(
        "build_transit_dijkstra_solver", [](TransitGraph& graph) { return TransitDijkstra(graph); }, py::keep_alive<1, 0>());
}
