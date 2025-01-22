#pragma once

#include <string>

#include "./parser.h"
#include "./proto/osmformat.pb.h"

namespace osm
{
class NodeTagMap : public TagMap
{
public:
    const OSMPBF::StringTable& table;
    const Span<uint32_t> keys;
    const Span<uint32_t> values;

public:
    NodeTagMap(const OSMPBF::StringTable& table, const Span<uint32_t> keys, const Span<uint32_t> values) : table(table), keys(keys), values(values) {}

    bool contains(const std::string& key) const
    {
        for (int i = 0; i < keys.size(); i++) {
            if (table.s(this->keys[i]) == key) {
                return true;
            }
        }
        return false;
    }

    const std::string& get(const std::string& key, const std::string& _default) const
    {
        for (int i = 0; i < keys.size(); i++) {
            if (table.s(keys[i]) == key) {
                return table.s(values[i]);
            }
        }
        return _default;
    }
};

class DenseNodeTagMap : public TagMap
{
private:
    const OSMPBF::StringTable& table;
    const Span<int32_t> keys;
    const Span<int32_t> values;

public:
    DenseNodeTagMap(const OSMPBF::StringTable& table, const Span<int32_t> keys, const Span<int32_t> values) : table(table), keys(keys), values(values) {}

    bool contains(const std::string& key) const
    {
        for (int i = 0; i < keys.size(); i++) {
            if (table.s(this->keys[i]) == key) {
                return true;
            }
        }
        return false;
    }

    const std::string& get(const std::string& key, const std::string& _default) const
    {
        for (int i = 0; i < keys.size(); i++) {
            if (table.s(keys[i]) == key) {
                return table.s(values[i]);
            }
        }
        return _default;
    }
};

}  // namespace osm
