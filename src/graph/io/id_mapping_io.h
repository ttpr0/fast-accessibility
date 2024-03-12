#pragma once

#include <memory>
#include <string>

#include "../base/id_mapping.h"

std::shared_ptr<_IDMapping> load_id_mapping(const std::string& file);

void store_id_mapping(const _IDMapping& id_mapping, const std::string& file);
