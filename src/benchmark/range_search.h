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
    Results results("Range");

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

        auto flags = DistFlagArray(graph->nodeCount());

        // run benchmakrs
        auto bench = nanobench::Bench();
        bench.run("Range-Dijkstra", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                flags.soft_reset();
                calcAllDijkstra(graph, start, flags);
            }
        });
        bench.run("PHAST", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                flags.soft_reset();
                calcPHAST(ch_graph, start, flags);
            }
        });
        bench.run("Range-PHAST", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                flags.soft_reset();
                calcRangePHAST(ch_graph, start, flags, range);
            }
        });
        bench.run("Range-PHAST2", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                flags.soft_reset();
                calcRangePHAST4(ch_graph, start, flags, range);
            }
        });
        bench.run("Range-PHAST2", [&] {
            for (int i = 0; i < N; i++) {
                int start = start_nodes[i];
                flags.soft_reset();
                calcRangePHAST2(ch_graph, start, flags, range);
            }
        });

        // gather results
        for (auto result : bench.results()) {
            auto name = result.config().mBenchmarkName;

            double time = result.average(nanobench::Result::Measure::elapsed);
            results.addResult(range, name, time * 1000 / N);
        }
    }

    // write results to file
    write_results("results.csv", results);
}