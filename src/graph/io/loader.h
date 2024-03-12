#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

#include "../base_graph.h"
#include "../ch_graph.h"
#include "../tiled_graph.h"

Graph loadGraph(const std::string& path);

CHGraph loadCHGraph(const std::string& path);

TiledGraph loadTiledGraph(const std::string& path);

CHGraph2 loadCHGraph2(const std::string& path);
