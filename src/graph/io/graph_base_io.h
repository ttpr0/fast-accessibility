#pragma once

#include <memory>
#include <string>

#include "../base/graph_base.h"

std::shared_ptr<GraphBase> load_graph_base(const std::string& file);

void store_graph_base(const GraphBase& base, const std::string& file);
