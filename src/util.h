#pragma once

#include <tuple>

template<class T>
class IIterator
{
public:
    virtual ~IIterator() {}

    virtual std::tuple<T, bool> next() = 0;
};
