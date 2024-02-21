#pragma once

#include <iostream>
#include <vector>

#include <nanobind/eigen/dense.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unique_ptr.h>

#include "accessibility/2sfca.h"
#include "accessibility/matrix_query.h"
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
    m.def("calc_2sfca", &calc2SFCA<RangeDijkstra>);
    m.def("calc_2sfca", &calc2SFCA<RangePHAST>);
    m.def("calc_2sfca", &calc2SFCA<RangeRPHAST>);
    m.def("calc_2sfca", &calc2SFCA<RangeRPHAST2>);
    m.def("calc_2sfca", &calc2SFCA<RangePHASTGS>);
    m.def("calc_2sfca", &calc2SFCA<RangeRPHASTGS>);
    m.def("calc_2sfca", &calc2SFCA<GRASP>);
    m.def("calc_2sfca", &calc2SFCA<RangeDijkstraTC>);

    // range query functions
    m.def("calc_range_query", &calcRangeQuery<RangeDijkstra>);
    m.def("calc_range_query", &calcRangeQuery<RangePHAST>);
    m.def("calc_range_query", &calcRangeQuery<RangeRPHAST>);
    m.def("calc_range_query", &calcRangeQuery<RangeRPHAST2>);
    m.def("calc_range_query", &calcRangeQuery<RangePHASTGS>);
    m.def("calc_range_query", &calcRangeQuery<RangeRPHASTGS>);
    m.def("calc_range_query", &calcRangeQuery<GRASP>);
    m.def("calc_range_query", &calcRangeQuery<RangeDijkstraTC>);

    // matrix query functions
    m.def("calc_matrix_query", &calcMatrixQuery<RangeDijkstra>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangePHAST>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangeRPHAST>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangeRPHAST2>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangePHASTGS>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangeRPHASTGS>);
    m.def("calc_matrix_query", &calcMatrixQuery<GRASP>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangeDijkstraTC>);

    // reachability functions
    m.def("calc_reachability", &calcReachability<RangeDijkstra>);
    m.def("calc_reachability", &calcReachability<RangePHAST>);
    m.def("calc_reachability", &calcReachability<RangeRPHAST>);
    m.def("calc_reachability", &calcReachability<RangeRPHAST2>);
}
