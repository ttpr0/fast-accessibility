#include <bit>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

#include <zlib.h>

#include "./internal.h"
#include "./parser.h"
#include "./proto/fileformat.pb.h"
#include "./proto/osmformat.pb.h"

#define MAX_BLOBHEADER_SIZE 64 * 1024
#define MAX_BLOB_SIZE 32 * 1024 * 1024

int32_t ntoh(int32_t network)
{
    if constexpr (std::endian::native == std::endian::big) {
        return network;
    } else {
        int32_t host = network;
        host = (host & 0x0000FFFF) << 16 | (host & 0xFFFF0000) >> 16;
        host = (host & 0x00FF00FF) << 8 | (host & 0xFF00FF00) >> 8;
        return host;
    }
}

namespace osm
{

void parse_osm(const std::string& filename, const std::vector<std::reference_wrapper<IHandler>>& handlers)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::vector<char> buffer(MAX_BLOB_SIZE);

    std::ifstream file(filename, std::ios::binary | std::ios::in);
    // std::streamsize size = file.tellg();
    // file.seekg(0, std::ios::beg);

    for (IHandler& handler : handlers) {
        while (true) {
            int32_t header_size;  // Documentation states this is supposed to be a int4 but reference Java implementation reads int as well
            if (!file.read(reinterpret_cast<char*>(&header_size), 4)) {
                break;
            }
            header_size = ntoh(header_size);
            if (header_size > MAX_BLOBHEADER_SIZE) {
                std::cout << "Header size too large" << std::endl;
                return;
            }
            if (!file.read(buffer.data(), header_size)) {
                std::cout << "Failed to read header" << std::endl;
                return;
            }
            OSMPBF::BlobHeader header;
            if (!header.ParseFromArray(buffer.data(), header_size)) {
                std::cout << "Failed to parse header" << std::endl;
                return;
            }
            const std::string blob_type = header.type();
            const int32_t blob_size = header.datasize();

            if (blob_size > MAX_BLOB_SIZE) {
                std::cout << "Blob size too large" << std::endl;
                return;
            }
            if (!file.read(buffer.data(), blob_size)) {
                std::cout << "Failed to read blob" << std::endl;
                return;
            }
            OSMPBF::Blob blob;
            if (!blob.ParseFromArray(buffer.data(), blob_size)) {
                std::cout << "Failed to parse blob" << std::endl;
                return;
            }
            int32_t raw_size = blob.raw_size();
            auto data_case = blob.data_case();
            if (data_case == OSMPBF::Blob::kRaw) {
                const auto& data = blob.raw();
                memcpy(buffer.data(), data.data(), data.size());
                raw_size = (int32_t)data.size();
            } else if (data_case == OSMPBF::Blob::kZlibData) {
                const auto zlib_data = blob.zlib_data();
                auto zlib_data_size = zlib_data.size();

                // z_stream zstream;
                // zstream.zalloc = Z_NULL;
                // zstream.zfree = Z_NULL;
                // zstream.opaque = Z_NULL;
                // zstream.next_in = (Bytef*)zlib_data.data();
                // zstream.avail_in = zlib_data_size;
                // zstream.next_out = (Bytef*)buffer.data();
                // zstream.avail_out = raw_size;

                // if (inflateInit(&zstream) != Z_OK) {
                //     std::cout << "Failed to initialize zlib decompressor" << std::endl;
                //     return;
                // }
                // if (inflate(&zstream, Z_FINISH) != Z_STREAM_END) {
                //     std::cout << "Failed to decompress blob" << std::endl;
                //     return;
                // }
                // if (inflateEnd(&zstream) != Z_OK) {
                //     std::cout << "Failed to finalize zlib decompressor" << std::endl;
                //     return;
                // }

                uLongf dest_size = raw_size;
                auto err = uncompress((Bytef*)buffer.data(), &dest_size, (const Bytef*)zlib_data.data(), (uLong)zlib_data.size());
                if (err != Z_OK) {
                    std::cout << "Failed to decompress blob: " << err << std::endl;
                    return;
                }
            } else {
                std::cout << "Unknown blob data case" << std::endl;
                return;
            }

            if (blob_type == "OSMHeader") {
                OSMPBF::HeaderBlock block;
                if (!block.ParseFromArray(buffer.data(), raw_size)) {
                    std::cout << "Failed to parse header block" << std::endl;
                    return;
                }
            } else if (blob_type == "OSMData") {
                OSMPBF::PrimitiveBlock block;
                if (!block.ParseFromArray(buffer.data(), raw_size)) {
                    std::cout << "Failed to parse primitive block" << std::endl;
                    return;
                }
                const auto& stringtable = block.stringtable();
                const auto& primitivegroups = block.primitivegroup();
                int32_t granularity = block.granularity();
                int64_t lat_offset = block.lat_offset();
                int64_t lon_offset = block.lon_offset();
                for (const auto& primitivegroup : primitivegroups) {
                    for (const auto& node : primitivegroup.nodes()) {
                        int64_t id = node.id();
                        int64_t ilat = node.lat();
                        int64_t ilon = node.lon();
                        double lat = .000000001 * (lat_offset + (granularity * ilat));
                        double lon = .000000001 * (lon_offset + (granularity * ilon));
                        const auto& keys = node.keys();
                        const auto& values = node.vals();
                        NodeTagMap tags(stringtable, {keys.data(), keys.size()}, {values.data(), values.size()});
                        Node n{id, lat, lon, tags};
                        handler.node(n);
                    }
                    for (const auto& way : primitivegroup.ways()) {
                        int64_t id = way.id();
                        const auto& refs = way.refs();
                        const auto& keys = way.keys();
                        const auto& values = way.vals();
                        NodeTagMap tags(stringtable, {keys.data(), keys.size()}, {values.data(), values.size()});
                        WayRefs way_refs = {{refs.data(), refs.size()}};
                        Way w{id, way_refs, tags};
                        handler.way(w);
                    }
                    if (primitivegroup.has_dense()) {
                        const auto& dense_node = primitivegroup.dense();
                        const auto& ids = dense_node.id();
                        const auto& lats = dense_node.lat();
                        const auto& lons = dense_node.lon();
                        const auto& keys_vals = dense_node.keys_vals();
                        int64_t id = 0;
                        double lat = 0;
                        double lon = 0;
                        int start = 0;
                        int end = 0;
                        for (int i = 0; i < ids.size(); i++) {
                            id += ids[i];
                            int64_t ilat = lats[i];
                            int64_t ilon = lons[i];
                            lat += .000000001 * (lat_offset + (granularity * ilat));
                            lon += .000000001 * (lon_offset + (granularity * ilon));
                            for (int j = start + 1; j < keys_vals.size(); j++) {
                                if (keys_vals[j] == '0') {
                                    end = j;
                                }
                            }
                            Span<int32_t> keys(nullptr, 0);
                            Span<int32_t> vals(nullptr, 0);
                            if (start != end) {
                                int size = (end - start) / 2;
                                const int* data = keys_vals.data();
                                Span<int32_t> keys(data + start, size, 2);
                                Span<int32_t> vals(data + start + 1, size, 2);
                            }
                            DenseNodeTagMap tags(stringtable, keys, vals);
                            Node n{id, lat, lon, tags};
                            handler.node(n);
                            start = end + 1;
                        }
                    }
                }
            }
        }
        file.clear();
        file.seekg(0, std::ios::beg);
    }
}
}  // namespace osm
