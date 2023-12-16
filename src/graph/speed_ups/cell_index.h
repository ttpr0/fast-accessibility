#pragma once

#include <unordered_map>
#include <vector>

#include "../structs/structs.h"

class _CellIndex
{
public:
    std::unordered_map<short, std::vector<Shortcut>> fwd_index_edges;
    std::unordered_map<short, std::vector<Shortcut>> bwd_index_edges;

    std::vector<Shortcut>& get_fwd_index(short tile) { return this->fwd_index_edges[tile]; }
    std::vector<Shortcut>& get_bwd_index(short tile) { return this->bwd_index_edges[tile]; }
    void _set_fwd_index(short tile, std::vector<Shortcut> edges) { this->fwd_index_edges[tile] = edges; }
    void _set_bwd_index(short tile, std::vector<Shortcut> edges) { this->bwd_index_edges[tile] = edges; }
};
