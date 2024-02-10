#pragma once

#include <memory>
#include <string>

#include "../weights/tc_weighting.h"
#include "../weights/weighting.h"

std::shared_ptr<Weighting> load_edge_weights(const std::string& file);

void store_edge_weights(const Weighting& weight, const std::string& file);

std::shared_ptr<TCWeighting> load_tc_weights(const std::string& file);

void store_tc_weights(const TCWeighting& weight, const std::string& file);
