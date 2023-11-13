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

void benchmark_study_area(ICHGraph* ch_graph, CHGraph2* ch_graph_2, ITiledGraph* tiled_graph)
{
    std::string point_dir = "./data/point_files";
    std::vector<std::tuple<int, std::string>> study_areas;
    study_areas.push_back({1345, "wilhelmshaven"});
    study_areas.push_back({1711, "delmenhorst"});
    study_areas.push_back({2126, "oldenburg_(oldb)"});
    study_areas.push_back({2618, "helmstedt"});
    study_areas.push_back({3062, "friesland"});
    study_areas.push_back({3443, "hameln-pyrmont"});
    study_areas.push_back({3901, "stade"});
    study_areas.push_back({4563, "vechta"});
    study_areas.push_back({5155, "gifhorn"});
    study_areas.push_back({5491, "nienburg_(weser)"});
    study_areas.push_back({6165, "celle"});
    study_areas.push_back({6447, "diepholz"});
    study_areas.push_back({7779, "region_hannover"});
    std::unordered_map<int, std::tuple<std::vector<Coord>, std::vector<int>, std::vector<Coord>, std::vector<int>>>
        study_area_points;
    int min_dem_count = 100000000;
    int min_sup_count = 100000000;
    for (auto [area, name] : study_areas) {
        auto dem_file = point_dir + "/population_" + name + ".txt";
        auto sup_file = point_dir + "/physicians_" + name + ".txt";
        // load demand and supply
        auto [dem_points, dem_weights] = read_points(dem_file);
        auto [sup_points, sup_weights] = read_points(sup_file);
        study_area_points[area] = std::make_tuple(dem_points, dem_weights, sup_points, sup_weights);
        if (min_dem_count > dem_points.size()) {
            min_dem_count = dem_points.size();
        }
        if (min_sup_count > sup_points.size()) {
            min_sup_count = sup_points.size();
        }
    }
    int sup_count = min_sup_count;
    int dem_count = min_dem_count;
    const int MAX_RANGE = 2400;
    std::cout << "start benchmark with: " << sup_count << " supply points and " << dem_count << " demand points"
              << std::endl;

    // create random benchmark data
    const int N = 1;
    std::unordered_map<
        int, std::vector<std::tuple<std::vector<Coord>, std::vector<int>, std::vector<Coord>, std::vector<int>>>>
        view_subsets;
    for (auto [area, _] : study_areas) {
        std::vector<std::tuple<std::vector<Coord>, std::vector<int>, std::vector<Coord>, std::vector<int>>> views;
        // get demand and supply
        auto [dem_points, dem_weights, sup_points, sup_weights] = study_area_points[area];
        for (int i = 0; i < N; i++) {
            auto [s_p, s_w] = select_random(sup_points, sup_weights, sup_count);
            auto [d_p, d_w] = select_random(dem_points, dem_weights, dem_count);
            views.push_back(std::make_tuple(std::move(d_p), std::move(d_w), std::move(s_p), std::move(s_w)));
        }
        view_subsets[area] = views;
    }

    // init results
    Results results("Study Area");

    // compute benchmarks for every value
    for (int i = 0; i < study_areas.size(); i++) {
        // get current test value
        auto [area, _] = study_areas[i];
        auto& views = view_subsets[area];

        // run benchmakrs
        auto bench = nanobench::Bench();
        bench.run("Range-Dijkstra", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcDijkstra2SFCA(ch_graph, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });
        bench.run("RPHAST", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcRPHAST2SFCA(ch_graph, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });
        bench.run("Range-RPHAST", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcRangeRPHAST2SFCA2(ch_graph, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });
        bench.run("GS-RPHAST", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcGSRPHAST2SFCA(ch_graph_2, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });
        bench.run("isoPHAST", [&] {
            for (int i = 0; i < N; i++) {
                auto& [d_p, d_w, s_p, s_w] = views[i];
                calcGRASP2SFCA(tiled_graph, d_p, d_w, s_p, s_w, MAX_RANGE);
            }
        });

        // gather results
        for (auto result : bench.results()) {
            auto name = result.config().mBenchmarkName;
            double time = result.average(nanobench::Result::Measure::elapsed);
            results.addResult(area, name, time * 1000 / N);
        }
    }

    // write results to file
    write_results("results_study_area.csv", results);
}