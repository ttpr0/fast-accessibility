#include <vector>

#include "../../util/io.h"
#include "./id_mapping_io.h"

std::shared_ptr<_IDMapping> load_id_mapping(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    auto mapping = reader.read<std::vector<std::array<int, 2>>>();

    return std::make_shared<_IDMapping>(mapping);
}

void store_id_mapping(const _IDMapping& id_mapping, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write<std::vector<std::array<int, 2>>>(id_mapping.mapping);

    writeFile(file, writer.bytes());
}
