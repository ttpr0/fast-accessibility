#include <vector>

#include "./id_mapping_io.h"
#include "./util.h"

std::shared_ptr<_IDMapping> load_id_mapping(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    auto mapping = reader.read_vector<std::array<int, 2>>();

    return std::make_shared<_IDMapping>(mapping);
}

void store_id_mapping(const _IDMapping& id_mapping, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write_vector<std::array<int, 2>>(id_mapping.mapping);

    writeFile(file, writer.bytes());
}
