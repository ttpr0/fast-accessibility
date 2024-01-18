#pragma once

#include <string>
#include <vector>

#include "../structs/structs.h"
#include "./util.h"

std::vector<Shortcut> load_shortcuts(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    int shortcutcount = reader.read<int>();
    reader.read<bool>();
    reader.read<int>();

    std::vector<Shortcut> shortcuts(shortcutcount);
    for (int i = 0; i < shortcutcount; i++) {
        int from = reader.read<int>();
        int to = reader.read<int>();
        int weight = reader.read<int>();
        auto payload = reader.read<Data<4>>();

        shortcuts[i] = Shortcut{from, to, weight, payload};
    }

    return shortcuts;
}

void store_shortcuts(std::vector<Shortcut>& shortcuts, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write<int>(shortcuts.size());
    writer.write<bool>(false);
    writer.write<int>(0);

    for (int i = 0; i < shortcuts.size(); i++) {
        auto shc = shortcuts[i];
        writer.write<int>(shc.from);
        writer.write<int>(shc.to);
        writer.write<int>(shc.weight);
        writer.write<Data<4>>(shc.payload);
    }

    writeFile(file, writer.bytes());
}