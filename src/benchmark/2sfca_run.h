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
#include "../accessibility/tiled_e2sfca.h"
#include "../algorithm/all_dijkstra.h"
#include "../algorithm/phast.h"
#include "../algorithm/range_phast.h"
#include "../graph/graph.h"
#include "./benchmark_util.h"

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
namespace nanobench = ankerl::nanobench;

void benchmark_2sfca_run(std::string sup_file, std::string dem_file, int max_range, std::string out_name,
                         ICHGraph* ch_graph, CHGraph2* ch_graph_2, ITiledGraph* tiled_graph)
{
    // load demand and supply
    auto [dem_points, dem_weights] = read_points(dem_file);
    auto [sup_points, sup_weights] = read_points(sup_file);
    std::cout << "finished reading " << dem_points.size() << " demand and " << sup_points.size() << " supply points"
              << std::endl;

    // init results
    Results results("2SFCA Run");

    // compute benchmarks
    auto bench = nanobench::Bench();
    bench.run("Range-Dijkstra",
              [&] { calcDijkstra2SFCA(ch_graph, dem_points, dem_weights, sup_points, sup_weights, max_range); });
    bench.run("RPHAST",
              [&] { calcRPHAST2SFCA(ch_graph, dem_points, dem_weights, sup_points, sup_weights, max_range); });
    bench.run("Range-RPHAST",
              [&] { calcRangeRPHAST2SFCA2(ch_graph, dem_points, dem_weights, sup_points, sup_weights, max_range); });
    bench.run("GS-RPHAST",
              [&] { calcGSRPHAST2SFCA(ch_graph_2, dem_points, dem_weights, sup_points, sup_weights, max_range); });
    bench.run("isoPHAST",
              [&] { calcGRASP2SFCA(tiled_graph, dem_points, dem_weights, sup_points, sup_weights, max_range); });

    // gather results
    for (auto result : bench.results()) {
        auto name = result.config().mBenchmarkName;
        double time = result.average(nanobench::Result::Measure::elapsed);
        results.addResult(0, name, time * 1000);
    }

    // write results to file
    write_results(out_name, results);
}