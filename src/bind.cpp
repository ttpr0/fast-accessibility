#include <iostream>

#include <nanobind/nanobind.h>

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
