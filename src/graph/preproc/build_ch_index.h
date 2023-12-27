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

// CHData should be reordered by level.
// Node-Ordering of GraphBase and Weighting are source CHData is target of id_mapping.
std::shared_ptr<_CHIndex> build_ch_index(GraphBase& base, Weighting& weight, CHData& ch, _IDMapping& id_mapping);

// CHData has to be reordered by tile and level.
// Node-Ordering of GraphBase, Weighting and Partition are source CHData is target of id_mapping.
std::shared_ptr<_CHIndex2> build_ch_index_2(GraphBase& base, Weighting& weight, CHData& ch, Partition& partition, _IDMapping& id_mapping);
