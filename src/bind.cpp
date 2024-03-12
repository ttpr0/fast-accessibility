#include <iostream>

#include <nanobind/eigen/dense.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unique_ptr.h>
#include <nanobind/stl/vector.h>

#include "binding/bind_accessibility.h"
#include "binding/bind_decay.h"
#include "binding/bind_graph.h"
#include "binding/bind_solver.h"
#include "binding/bind_utilities.h"

namespace py = nanobind;
using namespace std;

NB_MODULE(_pyaccess_ext, m)
{
    bind_utilities(m);
    bind_graph(m);
    bind_decay(m);
    bind_solver(m);
    bind_accessibility(m);
}
