#pragma once

#include <memory>
#include <string>

#include "../speed_ups/partition.h"

std::shared_ptr<Partition> load_node_partition(const std::string& file);

void store_node_partition(const Partition& partition, const std::string& file);
