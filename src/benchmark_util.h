#pragma once

#include <time.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "graph/graph.h"
#include "graph/loader.h"

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

std::tuple<std::vector<Coord>, std::vector<int>> select_random(std::vector<Coord> points, std::vector<int> weights, int count)
{
    std::vector<Coord> new_locs(count);
    std::vector<int> new_weights(count);

    int length = points.size();
    std::vector<std::tuple<int, int>> perm(length);
    srand(time(0));
    for (int i = 0; i < length; i++) {
        perm[i] = std::make_tuple(i, rand() % length);
    }
    std::sort(perm.begin(), perm.end(), [](std::tuple<int, int> a, std::tuple<int, int> b) { return std::get<1>(a) < std::get<1>(b); });
    for (int i = 0; i < count; i++) {
        int index = std::get<0>(perm[i]);
        new_locs[i] = points[index];
        new_weights[i] = weights[index];
    }

    return std::make_tuple(std::move(new_locs), std::move(new_weights));
}

void write_results(const std::string& filename, std::vector<std::tuple<int, std::vector<int>>>& results, std::vector<std::string>& headers)
{
    std::ofstream file;
    file.open(filename);

    // write headers
    for (int j = 0; j < headers.size() - 1; j++) {
        file << headers[j] << ";";
    }
    file << headers[headers.size() - 1];
    file << std::endl;

    // write results
    for (int i = 0; i < results.size(); i++) {
        auto [count, times] = results[i];
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
    srand(time(0));
    for (int i = 0; i < length; i++) {
        perm[i] = std::make_tuple(i, rand() % length);
    }
    std::sort(perm.begin(), perm.end(), [](std::tuple<int, int> a, std::tuple<int, int> b) { return std::get<1>(a) < std::get<1>(b); });
    for (int i = 0; i < count; i++) {
        int index = std::get<0>(perm[i]);
        new_nodes[i] = nodes[index];
    }

    return std::move(new_nodes);
}