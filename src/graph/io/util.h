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

std::vector<char> readAllFile(std::string filename)
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

std::vector<char> _readAllFile(std::string filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> arr(size);

    // allocate buffer
    char* buffer = arr.data();

    //read file
    if (!file.read(buffer, size)) {
        throw std::runtime_error("Failed to read file");
    }

    return arr;
}

void writeFile(const std::string& filename, std::vector<char>& data)
{
    auto myfile = std::fstream(filename, std::ios::out | std::ios::binary);
    myfile.write(&data[0], data.size());
    myfile.close();
}

class BufferReader
{
private:
    std::vector<char>& buffer;
    size_t offset;

public:
    BufferReader(std::vector<char>& buffer) : buffer(buffer), offset(0) {}

    template <typename T>
    T read()
    {
        int size = sizeof(T);
        T item = *(reinterpret_cast<T*>(&buffer[this->offset]));
        this->offset += size;
        return item;
    }

    template <typename T>
    std::vector<T> read_vector()
    {
        int size = sizeof(T);
        int count = *(reinterpret_cast<int*>(&this->buffer[this->offset]));
        std::vector<T> arr(count);
        for (int i = 0; i < count; i++) {
            T item = *(reinterpret_cast<T*>(&buffer[this->offset + 4 + i * size]));
            arr[i] = item;
        }
        this->offset += 4 + count * size;
        return arr;
    }
};

class BufferWriter
{
private:
    std::vector<char> buffer;

public:
    BufferWriter() {}

    std::vector<char>& bytes() { return this->buffer; }

    template <typename T>
    void write(T item)
    {
        int size = sizeof(T);
        size_t offset = this->buffer.size();
        this->buffer.resize(offset + size);
        std::memcpy(&buffer[offset], &item, size);
    }

    template <typename T>
    void write_vector(std::span<T> arr)
    {
        int size = sizeof(T);
        int count = arr.size();
        size_t offset = this->buffer.size();
        this->buffer.resize(offset + 4 + count * size);
        std::memcpy(&buffer[offset], &count, sizeof(int));
        for (int i = 0; i < count; i++) {
            std::memcpy(&buffer[offset + 4 + i * size], &arr[i], size);
        }
    }
};

template <typename T>
std::vector<T> read_vector_from_file(const std::string& file)
{
    auto arr = readAllFile(file);
    auto reader = BufferReader(arr);

    std::vector<T> values = reader.read_vector<T>();
    return values;
}

template <typename T>
void write_vector_to_file(std::vector<T>& vec, const std::string& file)
{
    auto writer = BufferWriter();

    writer.write_vector<T>(vec);

    writeFile(file, writer.bytes());
}
