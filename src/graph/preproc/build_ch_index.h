#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "../comps/graph_base.h"
#include "../comps/id_mapping.h"
#include "../comps/weighting.h"
#include "../speed_ups/ch_data.h"
#include "../speed_ups/ch_index.h"
#include "../speed_ups/partition.h"
#include "../structs/structs.h"

std::shared_ptr<_CHIndex> build_ch_index(GraphBase& base, Weighting& weight, CHData& ch, _IDMapping& id_mapping);
std::shared_ptr<_CHIndex2> build_ch_index_2(GraphBase& base, Weighting& weight, CHData& ch, Partition& partition, _IDMapping& id_mapping);
