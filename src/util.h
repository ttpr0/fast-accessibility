#pragma once

#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <tuple>
#include <unordered_map>
#include <vector>

template <class T>
class IIterator
{
public:
    virtual ~IIterator() {}

    virtual std::tuple<T, bool> next() = 0;
};

// static std::chrono::system_clock::time_point time_start()
// {
//     return std::chrono::high_resolution_clock::now();
// }
// static int time_since(std::chrono::system_clock::time_point& start)
// {
//     auto stop = std::chrono::high_resolution_clock::now();
//     return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
// }

// struct spin_lock
// {
//     std::atomic<bool> lock_ = {false};

//     void lock()
//     {
//         for (;;) {
//             if (!lock_.exchange(true, std::memory_order_acquire)) {
//                 break;
//             }
//             while (lock_.load(std::memory_order_relaxed)) {
//                 __builtin_ia32_pause();
//             }
//         }
//     }

//     void unlock() { lock_.store(false, std::memory_order_release); }
// };

class RNG
{
private:
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist;

public:
    RNG(int min, int max, int seed)
    {
        this->rng = std::mt19937(seed);
        this->dist = std::uniform_int_distribution<std::mt19937::result_type>(min, max);
    }

    int rand() { return this->dist(this->rng); }
};
