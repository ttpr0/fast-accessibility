#pragma once

#include <memory>
#include <string>

#include "../speed_ups/ch_data.h"

std::shared_ptr<CHData> load_ch_data(const std::string& file);

void store_ch_data(const CHData& ch, const std::string& file);
