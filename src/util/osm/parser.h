#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <string>

namespace osm
{

template <class T>
class Span
{
public:
    const T* _data;
    const int _size;
    const int _stride;

public:
    Span(const T* data, int size) : _data(data), _size(size), _stride(1) {}
    Span(const T* data, int size, int stride) : _data(data), _size(size), _stride(stride) {}

    int size() const { return _size; }
    const T& operator[](int i) const { return _data[i * _stride]; }
};

class TagMap
{
public:
    virtual bool contains(const std::string& key) const = 0;

    virtual const std::string& get(const std::string& key, const std::string& _default) const = 0;
};

class WayRefs
{
private:
    const Span<int64_t> refs;

public:
    WayRefs(const Span<int64_t> refs) : refs(refs) {}

    int size() const { return this->refs.size(); }
    int64_t operator[](int i) const
    {
        int64_t id = 0;
        for (int j = 0; j <= i; j++) {
            id += this->refs[j];
        }
        return id;
    }
};

struct Node
{
    int64_t id;
    double lat;
    double lon;
    const TagMap& tags;
};

struct Way
{
    int64_t id;
    const WayRefs& refs;
    const TagMap& tags;
};

class IHandler
{
public:
    virtual void node(const Node& node) = 0;
    virtual void way(const Way& way) = 0;
};

void parse_osm(const std::string& filename, const std::vector<std::reference_wrapper<IHandler>>& handlers);

}  // namespace osm
