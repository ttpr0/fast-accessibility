#pragma once

#include <unordered_map>
#include <vector>

#include "../structs/structs.h"

class _CHIndex
{
public:
    std::vector<Shortcut> fwd_down_edges;
    std::vector<Shortcut> bwd_down_edges;

    std::vector<Shortcut>& get_fwd_index() { return this->fwd_down_edges; }
    std::vector<Shortcut>& get_bwd_index() { return this->bwd_down_edges; }
};
