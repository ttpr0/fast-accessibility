#pragma once

#include <memory>
#include <string>

#include "../speed_ups/tiled_data.h"

std::shared_ptr<TiledData> load_tiled_data(const std::string& file);

void store_tiled_data(const TiledData& tiled, const std::string& file);
