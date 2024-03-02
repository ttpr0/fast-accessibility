#pragma once

#include <memory>
#include <string>

#include "../speed_ups/transit_data.h"

std::shared_ptr<TransitData> load_transit_data(const std::string& file);

void store_transit_data(const TransitData& ch, const std::string& file);
