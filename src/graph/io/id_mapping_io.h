#pragma once

#include <string>

#include "../structs/id_mapping.h"
#include "./util.h"

_IDMapping load_id_mapping(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    auto mapping = reader.read_vector<std::array<int, 2>>();

    return {mapping};
}

void store_id_mapping(_IDMapping& id_mapping, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write_vector<std::array<int, 2>>(id_mapping.mapping);

    writeFile(file, writer.bytes());
}
