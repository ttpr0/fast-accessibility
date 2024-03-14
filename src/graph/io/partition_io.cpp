#include <vector>

#include "../../util/io.h"
#include "./partition_io.h"

std::shared_ptr<Partition> load_node_partition(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    auto tiles = reader.read<std::vector<short>>();

    return std::make_shared<Partition>(tiles);
}

void store_node_partition(const Partition& partition, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write(partition.node_tiles);

    writeFile(file, writer.bytes());
}
