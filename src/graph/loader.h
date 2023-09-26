#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

#include "./base_graph.h"
#include "./ch_graph.h"
#include "./geom.h"
#include "./structs.h"
#include "./tiled_graph.h"
#include "./topology_storage.h"

std::unique_ptr<Graph> loadGraph(std::string path);

std::unique_ptr<CHGraph> loadCHGraph(std::string path);

std::unique_ptr<TiledGraph> loadTiledGraph(const std::string& path);

std::unique_ptr<CHGraph2> loadCHGraph2(std::string path);