#pragma once

#include <time.h>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../graph/graph.h"

std::tuple<std::vector<Coord>, std::vector<int>> read_points(const std::string& filename)
{
    std::vector<Coord> points;
    std::vector<int> weights;

    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        float lon;
        float lat;
        int w;
        if (!(iss >> lon >> lat >> w)) {
            break;
        }
        points.push_back(Coord{lon, lat});
        weights.push_back(w);
    }

    return std::make_tuple(std::move(points), std::move(weights));
}

std::tuple<std::vector<Coord>, std::vector<int>> select_random(std::vector<Coord>& points, std::vector<int>& weights,
                                                               int count)
{
    std::vector<Coord> new_locs(count);
    std::vector<int> new_weights(count);

    int length = points.size();
    std::vector<std::tuple<int, int>> perm(length);
    RNG rng(0, length - 1, time(0));
    for (int i = 0; i < length; i++) {
        perm[i] = std::make_tuple(i, rng.rand());
    }
    std::sort(perm.begin(), perm.end(),
              [](std::tuple<int, int> a, std::tuple<int, int> b) { return std::get<1>(a) < std::get<1>(b); });
    for (int i = 0; i < count; i++) {
        int index = std::get<0>(perm[i]);
        new_locs[i] = points[index];
        new_weights[i] = weights[index];
    }

    return std::make_tuple(std::move(new_locs), std::move(new_weights));
}

class Results
{
private:
    std::string name;
    std::vector<std::string> headers;
    std::vector<int> iterations;
    std::unordered_map<int, std::unordered_map<std::string, std::tuple<double, int>>> values;

public:
    Results(std::string name) { this->name = name; }

    void addResult(int iteration, std::string name, double result)
    {
        if (!this->values.contains(iteration)) {
            this->iterations.push_back(iteration);
            this->values[iteration] = std::unordered_map<std::string, std::tuple<double, int>>();
        }
        auto& times = this->values[iteration];
        if (times.contains(name)) {
            auto [t, c] = times[name];
            t += result;
            c += 1;
            times[name] = std::make_tuple(t, c);
        } else {
            times[name] = std::make_tuple(result, 1);
            if (std::find(this->headers.begin(), this->headers.end(), name) == this->headers.end()) {
                this->headers.push_back(name);
            }
        }
    }

    std::tuple<std::vector<std::tuple<int, std::vector<int>>>, std::vector<std::string>> getAllResults()
    {
        std::vector<std::string> headers = {this->name};
        for (auto& header : this->headers) {
            headers.push_back(header);
        }

        std::vector<std::tuple<int, std::vector<int>>> results;
        for (int iteration : this->iterations) {
            std::vector<int> result_times(this->headers.size());
            auto& times = this->values[iteration];
            for (int j = 0; j < this->headers.size(); j++) {
                auto& bench_name = this->headers[j];
                auto [time, count] = times[bench_name];
                result_times[j] = time / count;
            }
            results.push_back(make_tuple(iteration, result_times));
        }

        return std::make_tuple(std::move(results), std::move(headers));
    }
};

void write_results(const std::string& filename, Results& results)
{
    std::ofstream file;
    file.open(filename);

    auto [result_list, headers] = results.getAllResults();

    // write headers
    for (int j = 0; j < headers.size() - 1; j++) {
        file << headers[j] << ";";
    }
    file << headers[headers.size() - 1];
    file << std::endl;

    // write results
    for (int i = 0; i < result_list.size(); i++) {
        auto [count, times] = result_list[i];
        file << count << ";";
        for (int j = 0; j < times.size() - 1; j++) {
            file << times[j] << ";";
        }
        file << times[times.size() - 1];
        file << std::endl;
    }

    file.close();
}

std::vector<int> select_random_subset(std::vector<int>& nodes, int count)
{
    std::vector<int> new_nodes(count);

    int length = nodes.size();
    std::vector<std::tuple<int, int>> perm(length);
    RNG rng(0, length - 1, time(0));
    for (int i = 0; i < length; i++) {
        perm[i] = std::make_tuple(i, rng.rand());
    }
    std::sort(perm.begin(), perm.end(),
              [](std::tuple<int, int> a, std::tuple<int, int> b) { return std::get<1>(a) < std::get<1>(b); });
    for (int i = 0; i < count; i++) {
        int index = std::get<0>(perm[i]);
        new_nodes[i] = nodes[index];
    }

    return new_nodes;
}