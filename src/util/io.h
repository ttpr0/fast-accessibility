#pragma once

#define _FILE_OFFSET_BITS 64
#define _LARGE_FILE_SOURCE 1

#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <span>
#include <tuple>
#include <vector>

//*******************************************
// util methods
//*******************************************

static std::vector<char> readAllFile(std::string filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    file.unsetf(std::ios::skipws);
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // create vector
    std::vector<char> arr;
    arr.reserve(size);

    //read file
    arr.insert(arr.begin(), std::istream_iterator<char>(file), std::istream_iterator<char>());

    return arr;
}

static void writeFile(const std::string& filename, const std::vector<char>& data)
{
    auto myfile = std::fstream(filename, std::ios::out | std::ios::binary);
    myfile.write(&data[0], data.size());
    myfile.close();
}

struct BufferReader
{
    const std::vector<char>& _buffer;
    size_t _offset;

    BufferReader(const std::vector<char>& buffer) : _buffer(buffer), _offset(0) {}

    template <typename T>
    T read()
    {
        auto readwrite = ReadWrite<T>();
        return readwrite.read(*this);
    }
};

struct BufferWriter
{
    std::vector<char> _buffer;

    BufferWriter() {}

    template <typename T>
    void write(T item)
    {
        auto readwrite = ReadWrite<T>();
        readwrite.write(*this, item);
    }

    std::vector<char>& bytes() { return this->_buffer; }
};

template <typename T>
struct ReadWrite
{
    void write(BufferWriter& writer, const T& item)
    {
        int size = sizeof(T);
        size_t offset = writer._buffer.size();
        writer._buffer.resize(offset + size);
        std::memcpy(&writer._buffer[offset], &item, size);
    }
    T read(BufferReader& reader)
    {
        int size = sizeof(T);
        T item = *(reinterpret_cast<const T*>(&reader._buffer[reader._offset]));
        reader._offset += size;
        return item;
    }
};

template <typename T>
struct ReadWrite<std::vector<T>>
{
    void write(BufferWriter& writer, const std::vector<T>& arr)
    {
        int size = sizeof(T);
        int count = arr.size();
        size_t offset = writer._buffer.size();
        writer._buffer.resize(offset + 4 + count * size);
        std::memcpy(&writer._buffer[offset], &count, sizeof(int));
        for (int i = 0; i < count; i++) {
            std::memcpy(&writer._buffer[offset + 4 + i * size], &arr[i], size);
        }
    }
    std::vector<T> read(BufferReader& reader)
    {
        int size = sizeof(T);
        int count = *(reinterpret_cast<const int*>(&reader._buffer[reader._offset]));
        std::vector<T> arr(count);
        for (int i = 0; i < count; i++) {
            T item = *(reinterpret_cast<const T*>(&reader._buffer[reader._offset + 4 + i * size]));
            arr[i] = item;
        }
        reader._offset += 4 + count * size;
        return arr;
    }
};

template <typename T>
std::vector<T> read_vector_from_file(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    std::vector<T> values = reader.read<std::vector<T>>();
    return values;
}

template <typename T>
void write_vector_to_file(const std::vector<T>& vec, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write(vec);

    writeFile(file, writer.bytes());
}
