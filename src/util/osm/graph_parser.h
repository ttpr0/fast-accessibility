#pragma once

#include <vector>

#include "../../graph/structs/structs.h"
#include "../matrix.h"

std::tuple<std::vector<Node>, std::vector<Edge>, Vector<int>, Vector<float>, Vector<int>> graph_from_osm(const std::string& file, const std::string& profile);
