#pragma once

#include <time.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../accessibility/dijkstra_e2sfca.h"
#include "../accessibility/other_e2sfca.h"
#include "../accessibility/phast_e2sfca.h"
#include "../algorithm/all_dijkstra.h"
#include "../algorithm/phast.h"
#include "../algorithm/range_phast.h"
#include "../graph/graph.h"
#include "../graph/loader.h"
#include "./benchmark_util.h"

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
namespace nanobench = ankerl::nanobench;

void benchmark_supply_count(IGraph* graph, ICHGraph* ch_graph)
{
    const std::string dem_file = "./data/population_wittmund.txt";
    const std::string sup_file = "./data/physicians_wittmund.txt";
    const std::vector<int> sup_counts = {10, 30, 50, 100, 150, 200};
    const int dem_count = 10000;
    const int MAX_RANGE = 1800;

    // load demand and supply
    auto [dem_points, dem_weights] = read_points(dem_file);
    auto [sup_points, sup_weights] = read_points(sup_file);

    // create random benchmark data
    const int N = 1;
    std::unordered_map<int, std::vector<std::tuple<std::vector<Coord>, std::vector<int>, std::vector<Coord>, std::vector<int>>>> view_subsets;
    for (auto sup_count : sup_counts) {
        std::vector<std::tuple<std::vector<Coord>, std::vector<int>, std::vector<Coord>, std::vector<int>>> views;
        for (int i = 0; i < N; i++) {
            auto [s_p, s_w] = select_random(sup_points, sup_weights, sup_count);
            auto [d_p, d_w] = select_random(dem_points, dem_weights, dem_count);
            views.push_back(std::make_tuple(std::move(d_p), std::move(d_w), std::move(s_p), std::move(s_w)));
        }
        view_subsets[sup_count] = views;
    }

    // init results
    std::vector<std::tuple<int, std::vector<int>>> results;
    std::vector<std::string> headers = {"Supply Count"};

    // compute benchmarks for every value
    // compute benchmarks for every value
    for (int i = 0; i < sup_counts.size(); i++) {
        // get current test value
        int sup_count = sup_counts[i];
        auto& views = view_subsets[sup_count];

        // run benchmakrs
        auto bench = nanobench::Bench();
        bench.run("Range-Dijkstra", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                _calcDijkstra2SFCA(graph, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });
        bench.run("Range-Dijkstra2", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcDijkstra2SFCA(graph, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });
        bench.run("Range-PHAST", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcRangePHAST2SFCA(ch_graph, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });
        bench.run("Range-RPHAST", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcRangeRPHAST2SFCA(ch_graph, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });
        bench.run("Range-RPHAST2", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcRangeRPHAST2SFCA2(ch_graph, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });
        bench.run("Range-RPHAST3", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcRangeRPHAST2SFCA3(ch_graph, d_p, d_w, s_p, s_w, MAX_RANGE);
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
        results.push_back(make_tuple(sup_count, times));
    }

    // write results to file
    write_results("results_supply_count.csv", results, headers);
}