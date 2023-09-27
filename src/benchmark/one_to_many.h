#pragma once

#include <time.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../algorithm/one_to_many.h"
#include "../algorithm/special_dijkstra.h"
#include "../graph/graph.h"
#include "../graph/loader.h"
#include "./benchmark_util.h"

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
namespace nanobench = ankerl::nanobench;

void benchmark_one_to_many(ICHGraph* ch_graph, ITiledGraph* tiled_graph)
{
    std::cout << "start one-to-many benchmark:" << std::endl;
    // init results
    std::vector<std::tuple<int, std::vector<int>>> results;
    std::vector<std::string> headers = {
        "Iteration", "RPHAST-Preprocessing", "RangeRPHAST-Preprocessing", "RangeDijkstra", "PHAST", "RPHAST", "RangePHAST", "RangeRPHAST", "RangeRPHAST2", "GRASP"};
    const int BENCH_COUNT = 9;

    std::cout << "create benchmark data..." << std::endl;

    // create random benchmark data
    const int N = 1;
    const int N2 = 3;
    const int N3 = 3;
    const int RANGE = 1800;
    const int B = std::pow(2, 20);
    const int T = std::pow(2, 14);
    std::vector<std::vector<int>> start_nodes;
    std::vector<std::vector<std::vector<bool>>> target_nodes;
    srand(time(0));
    for (int i = 0; i < N; i++) {
        // get center for ball query
        int center = rand() % ch_graph->nodeCount();
        // compute ball query
        auto dist = calcCountDijkstra(ch_graph, center, B);
        std::vector<int> nodes;
        for (int j = 0; j < ch_graph->nodeCount(); j++) {
            if (dist[j] < 10000000) {
                nodes.push_back(j);
            }
        }

        std::vector<std::vector<bool>> targets;
        for (int j = 0; j < N2; j++) {
            std::vector<bool> is_target(ch_graph->nodeCount());
            for (int k = 0; k < ch_graph->nodeCount(); k++) {
                is_target[k] = false;
            }
            auto subset = select_random_subset(nodes, T);
            for (auto node : subset) {
                is_target[node] = true;
            }
            targets.push_back(std::move(is_target));
        }
        target_nodes.push_back(std::move(targets));

        std::vector<int> starts;
        for (int j = 0; j < N3; j++) {
            int start = rand() % nodes.size();
            starts.push_back(start);
        }
        start_nodes.push_back(std::move(starts));
    }

    std::cout << "start benchmarks..." << std::endl;

    // compute benchmarks for every value
    for (int i = 0; i < N; i++) {
        // get current test value
        std::vector<int>& starts_list = start_nodes[i];
        std::vector<std::vector<bool>>& targets_list = target_nodes[i];

        // run benchmakrs
        std::vector<double> times(BENCH_COUNT);
        for (int j = 0; j < BENCH_COUNT; j++) {
            times[j] = 0;
        }
        for (int k = 0; k < N2; k++) {
            std::vector<bool>& targets = targets_list[k];
            for (int j = 0; j < N3; j++) {
                int start = starts_list[j];

                // create benchmark instance
                auto bench = nanobench::Bench();

                std::vector<DistLabel> flags(ch_graph->nodeCount());
                for (int n = 0; n < flags.size(); n++) {
                    flags[n] = {1000000000, false, -1};
                }
                short counter = 0;

                // benchmark rphast preprocessing
                std::vector<CHEdge> down_edges_subset;
                bench.run("RPHAST-Preprocessing", [&] {
                    std::vector<CHEdge> subset = preprocessRPHAST(ch_graph, targets);
                    down_edges_subset = std::move(subset);
                });

                // benchmark range-rphast preprocessing
                std::vector<CHEdge> down_edges_subset_2;
                bench.run("RangeRPHAST-Preprocessing", [&] {
                    std::vector<CHEdge> subset = preprocessRangeRPHAST(ch_graph, RANGE, targets);
                    down_edges_subset_2 = std::move(subset);
                });

                // benchmark restricted-dijkstra
                bench.run("Restricted-Dijkstra", [&] {
                    counter = (counter + 1) % 10000;
                    calcRangeDijkstra(ch_graph, start, flags, counter, RANGE);
                });

                // benchmark phast
                bench.run("PHAST", [&] {
                    counter = (counter + 1) % 10000;
                    calcPHAST(ch_graph, start, flags, counter);
                });

                // benchmark rphast

                bench.run("RPHAST", [&] {
                    counter = (counter + 1) % 10000;
                    calcRPHAST(ch_graph, start, flags, counter, down_edges_subset);
                });

                // benchmark range-phast
                bench.run("RangePHAST", [&] {
                    counter = (counter + 1) % 10000;
                    calcRangePHAST(ch_graph, start, flags, counter, RANGE);
                });

                // benchmark range-rphast with normal preprocessing
                bench.run("RangeRPHAST", [&] {
                    counter = (counter + 1) % 10000;
                    calcRangeRPHAST(ch_graph, start, flags, counter, RANGE, down_edges_subset);
                });

                // benchmark range-rphast with range preprocessing
                bench.run("RangeRPHAST2", [&] {
                    counter = (counter + 1) % 10000;
                    calcRangeRPHAST(ch_graph, start, flags, counter, RANGE, down_edges_subset_2);
                });

                // benchmark range-rphast with range preprocessing
                bench.run("GRASP", [&] {
                    counter = (counter + 1) % 10000;
                    calcGRASP(tiled_graph, start, flags, counter, RANGE);
                });

                auto& results_list = bench.results();
                for (int l = 0; l < results_list.size(); l++) {
                    auto result = results_list[l];
                    double time = result.average(nanobench::Result::Measure::elapsed);
                    times[l] += time;
                }
            }
        }

        std::cout << "start writing results..." << std::endl;

        // gather results
        std::vector<int> result_times(BENCH_COUNT);
        for (int j = 0; j < BENCH_COUNT; j++) {
            result_times[j] = times[j] * 1000000 / (N2 * N3);
        }
        results.push_back(make_tuple(i, result_times));
    }

    // write results to file
    write_results("results.csv", results, headers);

    std::cout << "finished successfully!" << std::endl;
}