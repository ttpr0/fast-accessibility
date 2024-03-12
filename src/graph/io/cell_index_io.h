#pragma once

#include <memory>
#include <string>

#include "../speed_ups/cell_index.h"

std::shared_ptr<_CellIndex> load_cell_index(const std::string& file);

void store_cell_index(const _CellIndex& index, const std::string& file);
