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

#include "../algorithm/grasp.h"
#include "../algorithm/phast.h"
#include "../algorithm/range_dijkstra.h"
#include "../algorithm/range_gs_phast.h"
#include "../algorithm/range_gs_rphast.h"
#include "../algorithm/range_phast.h"
#include "../algorithm/range_rphast.h"
#include "../algorithm/rphast.h"
#include "../algorithm/rphast_preprocess.h"
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
    std::vector<std::string> headers = {"Iteration",   "RPHAST-Preprocessing", "RangeRPHAST-Preprocessing", "RangeDijkstra", "PHAST", "PHAST2", "RPHAST", "RangePHAST",
                                        "RangeRPHAST", "RangeRPHAST2"};
    const int BENCH_COUNT = 9;

    std::cout << "create benchmark data..." << std::endl;

    // create random benchmark data
    const int N = 1;
    const int N2 = 1;
    const int N3 = 1;
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
        auto flags = DistFlagArray(ch_graph->nodeCount());
        calcCountDijkstra(ch_graph, center, flags, B);
        std::vector<int> nodes;
        for (int j = 0; j < ch_graph->nodeCount(); j++) {
            if (flags[j].dist < 10000000) {
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

                DistFlagArray flags(ch_graph->nodeCount());

                // benchmark rphast preprocessing
                std::vector<CHEdge> down_edges_subset;
                bench.run("RPHAST-Preprocessing", [&] {
                    std::queue<int> node_queue;
                    for (int i = 0; i < ch_graph->nodeCount(); i++) {
                        if (targets[i]) {
                            node_queue.push(i);
                        }
                    }
                    std::vector<CHEdge> subset = preprocessRPHAST(ch_graph, std::move(node_queue));
                    down_edges_subset = std::move(subset);
                });

                // benchmark range-rphast preprocessing
                std::vector<CHEdge> down_edges_subset_2;
                bench.run("RangeRPHAST-Preprocessing", [&] {
                    std::priority_queue<pq_item> node_queue;
                    for (int i = 0; i < ch_graph->nodeCount(); i++) {
                        if (targets[i]) {
                            node_queue.push({i, 0});
                        }
                    }
                    std::vector<CHEdge> subset = preprocessRangeRPHAST(ch_graph, std::move(node_queue), RANGE);
                    down_edges_subset_2 = std::move(subset);
                });

                // benchmark restricted-dijkstra
                bench.run("Restricted-Dijkstra", [&] {
                    flags.soft_reset();
                    calcRangeDijkstra(ch_graph, start, flags, RANGE);
                });

                // test
                bench.run("Restricted-Dijkstra2", [&] {
                    flags.soft_reset();
                    calcRangeDijkstra2(ch_graph, start, flags, RANGE);
                });

                // benchmark phast
                bench.run("PHAST", [&] {
                    flags.soft_reset();
                    calcPHAST(ch_graph, start, flags);
                });

                // test
                bench.run("PHAST2", [&] {
                    flags.soft_reset();
                    calcPHAST2(ch_graph, start, flags);
                });

                // benchmark rphast
                bench.run("RPHAST", [&] {
                    flags.soft_reset();
                    calcRPHAST(ch_graph, start, flags, down_edges_subset);
                });

                // benchmark range-phast
                bench.run("RangePHAST", [&] {
                    flags.soft_reset();
                    calcRangePHAST(ch_graph, start, flags, RANGE);
                });

                // benchmark range-rphast with normal preprocessing
                bench.run("RangeRPHAST", [&] {
                    flags.soft_reset();
                    calcRangeRPHAST(ch_graph, start, flags, RANGE, down_edges_subset);
                });

                // benchmark range-rphast with range preprocessing
                bench.run("RangeRPHAST2", [&] {
                    flags.soft_reset();
                    calcRangeRPHAST(ch_graph, start, flags, RANGE, down_edges_subset_2);
                });

                // benchmark range-rphast with range preprocessing
                // bench.run("GRASP", [&] {
                //     counter = (counter + 1) % 10000;
                //     calcGRASP(tiled_graph, start, flags, counter, RANGE);
                // });

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