#pragma once

#include <vector>

#include "../../util/function_ref.h"
#include "../../util/io.h"

template <typename T>
class DynamicTable;

template <typename T>
class StaticTable;

template <typename T>
struct TableAccessor
{
    int state;
    int end;
    const T* val_refs;

    bool next()
    {
        if (this->state == this->end) {
            return false;
        }
        this->state += 1;
        return true;
    }
    T getValue() { return this->val_refs[this->state - 1]; }
};

template <typename T>
struct StaticTable
{
    struct Entry
    {
        int start;
        int count;
    };

    std::vector<Entry> entries;
    std::vector<T> values;

    StaticTable() = default;
    StaticTable(std::vector<Entry> entries, std::vector<T> values) : entries(entries), values(values) {}

    int len() const { return entries.size(); }
    int getCount(int i) const { return entries[i].count; }
    TableAccessor<T> getValues(int i) const
    {
        Entry entry = this->entries[i];
        const T* val_refs = &(this->values[entry.start]);
        int start = 0;
        int end = entry.count;
        return {start, end, val_refs};
    }

    StaticTable<T> reorderEntries(const std::vector<int>& mapping) const
    {
        return this->reorderEntries(mapping, [](T val) { return val; });
    }
    StaticTable<T> reorderEntries(const std::vector<int>& mapping, function_ref<T(T)> value_remap_handler) const
    {
        std::vector<Entry> entries(this->entries.size());
        for (int i = 0; i < this->entries.size(); i++) {
            int m_id = mapping[i];
            entries[m_id] = this->entries[i];
        }
        std::vector<T> values(this->values.size());
        int start = 0;
        for (int i = 0; i < entries.size(); i++) {
            auto& entry = entries[i];
            int count = 0;
            int start = entry.start;
            for (int j = 0; j < entry.count; j++) {
                auto val = this->values[start + j];
                values[start + count] = value_remap_handler(val);
                count += 1;
            }
            entry.start = count > 0 ? start : 0;
            entry.count = count;
            start += count;
        }

        return {entries, values};
    }
};

template <typename T>
struct ReadWrite<StaticTable<T>>
{
    void write(BufferWriter& writer, const StaticTable<T>& table)
    {
        writer.write<int>(table.entries.size());
        writer.write<int>(table.values.size());
        for (int i = 0; i < table.entries.size(); i++) {
            auto& entry = table.entries[i];
            writer.write<int>(entry.start);
            writer.write<int>(entry.count);
        }
        for (int i = 0; i < table.values.size(); i++) {
            T val = table.values[i];
            writer.write<T>(val);
        }
    }
    StaticTable<T> read(BufferReader& reader)
    {
        int size = reader.read<int>();
        int count = reader.read<int>();
        std::vector<StaticTable<T>::Entry> entries(size);
        std::vector<T> values(count);
        for (int i = 0; i < size; i++) {
            int s = reader.read<int>();
            int c = reader.read<int>();
            entries[i] = {s, c};
        }
        for (int i = 0; i < count; i++) {
            T val = reader.read<T>();
            values[i] = val;
        }
        return {entries, values};
    }
};

template <typename T>
struct DynamicTable
{
    struct Entry
    {
        std::vector<T> values;
    };

    std::vector<Entry> entries;

    DynamicTable() = default;
    DynamicTable(int size) : entries(size) {}

    int len() const { return entries.size(); }
    int getCount(int i) const
    {
        auto& entry = this->entries[i];
        if (entry.values.empty()) {
            return 0;
        }
        return entry.values.size();
    }
    TableAccessor<T> getValues(int i) const
    {
        auto& entry = this->entries[i];
        const T* val_refs;
        int start = 0;
        int end;
        if (entry.values.empty()) {
            val_refs = nullptr;
            end = 0;
        } else {
            val_refs = &(entry.values[0]);
            end = entry.values.size();
        }
        return {start, end, val_refs};
    }

    void addValue(int i, T value) { this->entries[i].values.push_back(value); }

    StaticTable<T> toStatic()
    {
        std::vector<StaticTable<T>::Entry> entries(this->entries.size());
        std::vector<T> values;
        for (int i = 0; i < this->entries.size(); i++) {
            Entry entry = this->entries[i];
            entries[i].start = values.size();
            if (entry.values.empty()) {
                entries[i].count = 0;
            } else {
                entries[i].count = entry.values.size();
            }
            values.insert(values.end(), entry.values.begin(), entry.values.end());
        }
        return StaticTable<T>(entries, values);
    }
};
