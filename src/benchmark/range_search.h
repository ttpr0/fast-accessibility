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