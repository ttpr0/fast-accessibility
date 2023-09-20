#pragma once

#include <time.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "accessibility/dijkstra_e2sfca.h"
#include "accessibility/phast_e2sfca.h"
#include "algorithm/all_dijkstra.h"
#include "algorithm/phast.h"
#include "algorithm/range_phast.h"
#include "graph/graph.h"
#include "graph/loader.h"

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
namespace nanobench = ankerl::nanobench;

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

void benchmark_range_search(IGraph* graph, ICHGraph* ch_graph)
{
    // init results
    std::vector<std::tuple<int, std::vector<int>>> results;
    std::vector<std::string> headers = {"Range"};

    // create random benchmark data
    const int N = 10;
    std::vector<int> start_nodes;
    srand(time(0));
    for (int i = 0; i < N; i++) {
        start_nodes.push_back(rand() % graph->nodeCount());
    }

    // values to test for
    std::vector<int> counts = {900, 1800, 2700};

    // compute benchmarks for every value
    for (int i = 0; i < counts.size(); i++) {
        // get current test value
        int range = counts[i];

        // run benchmakrs
        auto bench = nanobench::Bench();
        bench.run("Range-Dijkstra", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                calcAllDijkstra(graph, start, range);
            }
        });
        bench.run("PHAST", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                calcPHAST(ch_graph, start);
            }
        });
        bench.run("Range-PHAST", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                calcRangePHAST(ch_graph, start, range);
            }
        });
        bench.run("Range-PHAST2", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                calcRangePHAST4(ch_graph, start, range);
            }
        });
        bench.run("Range-PHAST3", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                calcRangePHAST3(ch_graph, start, range);
            }
        });

        // gather results
        std::vector<int> times;
        for (auto result : bench.results()) {
            auto name = result.config().mBenchmarkName;
            if (i == 0) {
                headers.push_back(name);
            }
            double time = result.average(nanobench::Result::Measure::elapsed);
            times.push_back(time * 1000 / N);
        }
        results.push_back(make_tuple(range, times));
    }

    // write results to file
    write_results("results.csv", results, headers);
}

void benchmark_supply_count(IGraph* graph, ICHGraph* ch_graph)
{
    // load demand and supply
    auto [dem_points, dem_weights] = read_points("./data/population_wittmund.txt");
    auto [sup_points, sup_weights] = read_points("./data/physicians_wittmund.txt");

    // create random benchmark data
    const int N = 5;
    int max_range = 2700;
    std::vector<int> counts = {10, 30, 50, 100, 150, 200};
    std::unordered_map<int, std::vector<std::tuple<std::vector<Coord>, std::vector<int>>>> supply_views;
    for (auto count : counts) {
        std::vector<std::tuple<std::vector<Coord>, std::vector<int>>> views;
        for (int i = 0; i < N; i++) {
            auto [locs, weights] = select_random(sup_points, sup_weights, count);
            views.push_back(std::make_tuple(std::move(locs), std::move(weights)));
        }
        supply_views[count] = views;
    }

    // init results
    std::vector<std::tuple<int, std::vector<int>>> results;
    std::vector<std::string> headers = {"Supply Count"};

    // compute benchmarks for every value
    for (int i = 0; i < counts.size(); i++) {
        // get current test value
        int count = counts[i];
        std::vector<std::tuple<std::vector<Coord>, std::vector<int>>> views = supply_views[count];

        // run benchmakrs
        auto bench = nanobench::Bench();
        bench.run("Range-Dijkstra", [&] {
            for (int i = 0; i < N; i++) {
                auto [s_p, s_w] = views[i];
                calcDijkstra2SFCA(graph, dem_points, dem_weights, s_p, s_w, max_range);
            }
        });
        bench.run("Range-PHAST", [&] {
            for (int i = 0; i < N; i++) {
                auto [s_p, s_w] = views[i];
                calcRangePHAST2SFCA(ch_graph, dem_points, dem_weights, s_p, s_w, max_range);
            }
        });
        bench.run("Range-RPHAST", [&] {
            for (int i = 0; i < N; i++) {
                auto [s_p, s_w] = views[i];
                calcRangeRPHAST2SFCA(ch_graph, dem_points, dem_weights, s_p, s_w, max_range);
            }
        });
        bench.run("Range-RPHAST2", [&] {
            for (int i = 0; i < N; i++) {
                auto [s_p, s_w] = views[i];
                calcRangeRPHAST2SFCA2(ch_graph, dem_points, dem_weights, s_p, s_w, max_range);
            }
        });
        bench.run("Range-RPHAST3", [&] {
            for (int i = 0; i < N; i++) {
                auto [s_p, s_w] = views[i];
                calcRangeRPHAST2SFCA3(ch_graph, dem_points, dem_weights, s_p, s_w, max_range);
            }
        });

        // gather results
        std::vector<int> times;
        for (auto result : bench.results()) {
            auto name = result.config().mBenchmarkName;
            if (i == 0) {
                headers.push_back(name);
            }
            double time = result.average(nanobench::Result::Measure::elapsed);
            times.push_back(time * 1000 / N);
        }
        results.push_back(make_tuple(count, times));
    }

    // write results to file
    write_results("results_supply_count.csv", results, headers);
}