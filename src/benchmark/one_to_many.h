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

void benchmark_one_to_many(ICHGraph* ch_graph)
{
    std::cout << "start one-to-many benchmark:" << std::endl;
    // init results
    std::vector<std::tuple<int, std::vector<int>>> results;
    std::vector<std::string> headers = {"Iteration", "Dijkstra", "PHAST", "RPHAST-Preprocessing", "RPHAST"};

    std::cout << "create benchmark data..." << std::endl;

    // create random benchmark data
    const int N = 1;
    const int N2 = 3;
    const int N3 = 3;
    const int RANGE = 1800;
    const int B = std::pow(2, 14);
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
        std::vector<double> times(4);
        for (int j = 0; j < times.size(); j++) {
            times[j] = 0;
        }
        for (int k = 0; k < N2; k++) {
            std::vector<bool>& targets = targets_list[k];
            for (int j = 0; j < N3; j++) {
                auto bench = nanobench::Bench();

                int start = starts_list[j];

                // benchmark restricted-dijkstra
                std::vector<int> dist(ch_graph->nodeCount());
                std::vector<bool> visited(ch_graph->nodeCount());
                int target_count = 0;
                for (int l = 0; l < ch_graph->nodeCount(); l++) {
                    dist[l] = 1000000000;
                    visited[l] = false;
                    if (targets[l]) {
                        target_count += 1;
                    }
                }
                bench.run("Restricted-Dijkstra", [&] { calcRestrictedRangeDijkstra(ch_graph, start, dist, visited, RANGE, targets, target_count); });

                // benchmark phast
                for (int l = 0; l < ch_graph->nodeCount(); l++) {
                    dist[l] = 1000000000;
                    visited[l] = false;
                }
                bench.run("PHAST", [&] { calcPHAST(ch_graph, start, dist, visited); });

                // benchmark rphast preprocessing
                std::vector<CHEdge> down_edges_subset;
                bench.run("RPHAST-Preprocessing", [&] {
                    std::vector<CHEdge> subset = preprocessRPHAST(ch_graph, targets);
                    down_edges_subset = std::move(subset);
                });

                // benchmark rphast
                for (int l = 0; l < ch_graph->nodeCount(); l++) {
                    dist[l] = 1000000000;
                    visited[l] = false;
                }
                bench.run("RPHAST", [&] { calcRPHAST(ch_graph, start, dist, visited, down_edges_subset); });

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
        std::vector<int> result_times(4);
        for (int j = 0; j < result_times.size(); j++) {
            result_times[j] = times[j] * 1000 / (N2 * N3);
        }
        results.push_back(make_tuple(i, result_times));
    }

    // write results to file
    write_results("results.csv", results, headers);

    std::cout << "finished successfully!" << std::endl;
}