#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <memory>

#include "./structs.h"
#include "./geom.h"
#include "./topology_storage.h"
#include "./base_graph.h"
#include "./ch_graph.h"

std::unique_ptr<Graph> loadGraph(std::string path);

std::unique_ptr<CHGraph> loadCHGraph(std::string path);
