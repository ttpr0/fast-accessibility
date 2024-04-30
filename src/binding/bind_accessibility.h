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

#include "../accessibility/2sfca.h"
#include "../accessibility/matrix_query.h"
#include "../accessibility/range_query.h"
#include "../accessibility/reachability.h"
#include "../accessibility/aggregation.h"
#include "../graph/graph.h"
#include "../solver/one_to_many/dijkstra.h"
#include "../solver/one_to_many/grasp.h"
#include "../solver/one_to_many/phast.h"
#include "../solver/one_to_many/rphast.h"
#include "../solver/one_to_many/transit.h"

void bind_accessibility(nanobind::module_& m)
{
    namespace py = nanobind;

    //*******************************************
    // accessibility functions
    //*******************************************

    // 2sfca functions
    m.def("calc_2sfca", &calc2SFCA<RangeDijkstra<>>);
    m.def("calc_2sfca", &calc2SFCA<RangePHAST<>>);
    m.def("calc_2sfca", &calc2SFCA<RangeRPHAST<>>);
    m.def("calc_2sfca", &calc2SFCA<RangeRPHAST2<>>);
    m.def("calc_2sfca", &calc2SFCA<RangePHASTGS>);
    m.def("calc_2sfca", &calc2SFCA<RangeRPHASTGS>);
    m.def("calc_2sfca", &calc2SFCA<GRASP<>>);
    m.def("calc_2sfca", &calc2SFCA<RangeDijkstraTC<>>);
    m.def("calc_2sfca", &calc2SFCA<TransitDijkstra>);

    // range query functions
    m.def("calc_range_query", &calcRangeQuery<RangeDijkstra<>>);
    m.def("calc_range_query", &calcRangeQuery<RangePHAST<>>);
    m.def("calc_range_query", &calcRangeQuery<RangeRPHAST<>>);
    m.def("calc_range_query", &calcRangeQuery<RangeRPHAST2<>>);
    m.def("calc_range_query", &calcRangeQuery<RangePHASTGS>);
    m.def("calc_range_query", &calcRangeQuery<RangeRPHASTGS>);
    m.def("calc_range_query", &calcRangeQuery<GRASP<>>);
    m.def("calc_range_query", &calcRangeQuery<RangeDijkstraTC<>>);
    m.def("calc_range_query", &calcRangeQuery<TransitDijkstra>);

    // matrix query functions
    m.def("calc_matrix_query", &calcMatrixQuery<RangeDijkstra<>>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangePHAST<>>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangeRPHAST<>>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangeRPHAST2<>>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangePHASTGS>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangeRPHASTGS>);
    m.def("calc_matrix_query", &calcMatrixQuery<GRASP<>>);
    m.def("calc_matrix_query", &calcMatrixQuery<RangeDijkstraTC<>>);
    m.def("calc_matrix_query", &calcMatrixQuery<TransitDijkstra>);

    // reachability functions
    m.def("calc_reachability", &calcReachability<RangeDijkstra<>>);
    m.def("calc_reachability", &calcReachability<RangePHAST<>>);
    m.def("calc_reachability", &calcReachability<RangeRPHAST<>>);
    m.def("calc_reachability", &calcReachability<RangeRPHAST2<>>);
    m.def("calc_reachability", &calcReachability<RangePHASTGS>);
    m.def("calc_reachability", &calcReachability<RangeRPHASTGS>);
    m.def("calc_reachability", &calcReachability<GRASP<>>);
    m.def("calc_reachability", &calcReachability<RangeDijkstraTC<>>);
    m.def("calc_reachability", &calcReachability<TransitDijkstra>);

    // closest functions
    m.def("calc_aggregation", &calcAggregation<RangeDijkstra<>>);
    m.def("calc_aggregation", &calcAggregation<RangePHAST<>>);
    m.def("calc_aggregation", &calcAggregation<RangeRPHAST<>>);
    m.def("calc_aggregation", &calcAggregation<RangeRPHAST2<>>);
    m.def("calc_aggregation", &calcAggregation<RangePHASTGS>);
    m.def("calc_aggregation", &calcAggregation<RangeRPHASTGS>);
    m.def("calc_aggregation", &calcAggregation<GRASP<>>);
    m.def("calc_aggregation", &calcAggregation<RangeDijkstraTC<>>);
    m.def("calc_aggregation", &calcAggregation<TransitDijkstra>);
}
