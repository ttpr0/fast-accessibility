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

void bind_accessibility(nanobind::module_& m)
{
    namespace py = nanobind;

    //*******************************************
    // accessibility functions
    //*******************************************

    // 2sfca functions
    m.def("calc_dijkstra_2sfca", &calc2SFCA<RangeDijkstra>);
    m.def("calc_range_phast_2sfca", &calc2SFCA<RangePHAST>);
    m.def("calc_range_rphast_2sfca", &calc2SFCA<RangeRPHAST>);
    m.def("calc_range_rphast_2sfca2", &calc2SFCA<RangeRPHAST2>);

    // range query functions
    m.def("calc_range_dijkstra", &calcRangeQuery<RangeDijkstra>);
    m.def("calc_range_phast", &calcRangeQuery<RangePHAST>);
    m.def("calc_range_rphast", &calcRangeQuery<RangeRPHAST>);
    m.def("calc_range_rphast_2", &calcRangeQuery<RangeRPHAST2>);
    m.def("calc_gs_phast", &calcRangeQuery<RangePHASTGS>);
    m.def("calc_gs_rphast", &calcRangeQuery<RangeRPHASTGS>);
    m.def("calc_grasp", &calcRangeQuery<GRASP>);

    // reachability functions
    m.def("calc_dijkstra_reachability", &calcReachability<RangeDijkstra>);
    m.def("calc_range_phast_reachability", &calcReachability<RangePHAST>);
    m.def("calc_range_rphast_reachability", &calcReachability<RangeRPHAST>);
    m.def("calc_range_rphast2_reachability", &calcReachability<RangeRPHAST2>);
}