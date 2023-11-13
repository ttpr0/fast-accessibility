#pragma once

#include <time.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "./2sfca_run.h"
#include "./catchment_range.h"
#include "./demand_count.h"
#include "./one_to_many.h"
#include "./study_area.h"
#include "./supply_count.h"

#include "../graph/graph.h"
#include "../graph/io/loader.h"

void run_benchmarks()
{
    const std::string GRAPH_DIR = "D:/Routing_Graphen/germany_2";
    const std::string GRAPH_NAME = "germany";
    // auto graph = loadGraph("./graphs/germany_ch");
    // auto ch_graph = loadCHGraph("D:/Routing_Graphen/niedersachsen/niedersachsen_ch_reordered");
    // auto ch_graph_1000 = loadCHGraph2("D:/Routing_Graphen/niedersachsen/niedersachsen_ch_tiled_1000");
    // auto grasp_graph_1000 = loadTiledGraph("D:/Routing_Graphen/niedersachsen/niedersachsen_tiled_1000_grasp");
    // auto isophast_graph_1000 = loadTiledGraph("D:/Routing_Graphen/niedersachsen/niedersachsen_tiled_1000_phast");

    std::cout << "start loading graphs..." << std::endl;
    auto ch_graph = loadCHGraph(GRAPH_DIR + "/" + GRAPH_NAME + "_ch");
    std::cout << "finished loading ch-graph..." << std::endl;

    {
        auto ch_graph_10000 = loadCHGraph2(GRAPH_DIR + "/" + GRAPH_NAME + "_ch_tiled_10000");
        std::cout << "finished loading ch-graph 10000..." << std::endl;
        auto isophast_graph_10000 = loadTiledGraph(GRAPH_DIR + "/" + GRAPH_NAME + "_isophast_10000");
        std::cout << "finished loading isophast-graph 10000..." << std::endl;

        // get benchmark graphs
        const auto CH_GRAPH = &ch_graph;
        const auto CH_GRAPH_TILED = &ch_graph_10000;
        const auto TILED_GRAPH = &isophast_graph_10000;

        // benchmark parameters
        benchmark_catchment_range(CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_supply_count(CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_demand_count(CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_study_area(CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);

        // benchmark hausarzt
        benchmark_2sfca_run("./data/physician/mittelbereiche/physicians_munster_hausarzt.txt",
                            "./data/points/mittelbereiche/population_munster.txt", 1930,
                            "results_2sfca_munster_hausarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/mittelbereiche/physicians_westerstede_hausarzt.txt",
                            "./data/points/mittelbereiche/population_westerstede.txt", 1930,
                            "results_2sfca_westerstede_hausarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/mittelbereiche/physicians_hildesheim_hausarzt.txt",
                            "./data/points/mittelbereiche/population_hildesheim.txt", 1930,
                            "results_2sfca_hildesheim_hausarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/physicians_niedersachsen_hausarzt.txt",
                            "./data/points/population_niedersachsen.txt", 1930,
                            "results_2sfca_niedersachsen_hausarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);

        // benchmark augenarzt
        benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_augenarzt.txt",
                            "./data/points/landkreise/population_emden.txt", 2413, "results_2sfca_emden_augenarzt.csv",
                            CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_augenarzt.txt",
                            "./data/points/landkreise/population_braunschweig.txt", 2413,
                            "results_2sfca_braunschweig_augenarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_augenarzt.txt",
                            "./data/points/landkreise/population_region_hannover.txt", 2413,
                            "results_2sfca_region_hannover_augenarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/physicians_niedersachsen_augenarzt.txt",
                            "./data/points/population_niedersachsen.txt", 2413,
                            "results_2sfca_niedersachsen_augenarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);

        // // benchmark frauenarzt
        benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_frauenarzt.txt",
                            "./data/points/landkreise/population_emden.txt", 2550, "results_2sfca_emden_frauenarzt.csv",
                            CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_frauenarzt.txt",
                            "./data/points/landkreise/population_braunschweig.txt", 2550,
                            "results_2sfca_braunschweig_frauenarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_frauenarzt.txt",
                            "./data/points/landkreise/population_region_hannover.txt", 2550,
                            "results_2sfca_region_hannover_frauenarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/physicians_niedersachsen_frauenarzt.txt",
                            "./data/points/population_niedersachsen.txt", 2550,
                            "results_2sfca_niedersachsen_frauenarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);

        // benchmark hautarzt
        benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_hautarzt.txt",
                            "./data/points/landkreise/population_emden.txt", 2802, "results_2sfca_emden_hautarzt.csv",
                            CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_hautarzt.txt",
                            "./data/points/landkreise/population_braunschweig.txt", 2802,
                            "results_2sfca_braunschweig_hautarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_hautarzt.txt",
                            "./data/points/landkreise/population_region_hannover.txt", 2802,
                            "results_2sfca_region_hannover_hautarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/physicians_niedersachsen_hautarzt.txt",
                            "./data/points/population_niedersachsen.txt", 2802,
                            "results_2sfca_niedersachsen_hautarzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);

        // // benchmark hno_arzt
        benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_hno_arzt.txt",
                            "./data/points/landkreise/population_emden.txt", 2477, "results_2sfca_emden_hno_arzt.csv",
                            CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_hno_arzt.txt",
                            "./data/points/landkreise/population_braunschweig.txt", 2477,
                            "results_2sfca_braunschweig_hno_arzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_hno_arzt.txt",
                            "./data/points/landkreise/population_region_hannover.txt", 2477,
                            "results_2sfca_region_hannover_hno_arzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/physicians_niedersachsen_hno_arzt.txt",
                            "./data/points/population_niedersachsen.txt", 2477,
                            "results_2sfca_niedersachsen_hno_arzt.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);

        // benchmark orthopade
        benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_orthopade.txt",
                            "./data/points/landkreise/population_emden.txt", 3076, "results_2sfca_emden_orthopade.csv",
                            CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_orthopade.txt",
                            "./data/points/landkreise/population_braunschweig.txt", 3076,
                            "results_2sfca_braunschweig_orthopade.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_orthopade.txt",
                            "./data/points/landkreise/population_region_hannover.txt", 3076,
                            "results_2sfca_region_hannover_orthopade.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
        benchmark_2sfca_run("./data/physician/physicians_niedersachsen_orthopade.txt",
                            "./data/points/population_niedersachsen.txt", 3076,
                            "results_2sfca_niedersachsen_orthopade.csv", CH_GRAPH, CH_GRAPH_TILED, TILED_GRAPH);
    }

    // benchmark range-query
    {
        auto ch_graph_1000 = loadCHGraph2(GRAPH_DIR + "/" + GRAPH_NAME + "_ch_tiled_1000");
        std::cout << "finished loading ch-graph 1000..." << std::endl;
        auto isophast_graph_1000 = loadTiledGraph(GRAPH_DIR + "/" + GRAPH_NAME + "_isophast_1000");
        std::cout << "finished loading isophast-graph 1000..." << std::endl;
        benchmark_one_to_many(&ch_graph, &ch_graph_1000, nullptr, &isophast_graph_1000, 14, 14, 1000);
        benchmark_one_to_many(&ch_graph, &ch_graph_1000, nullptr, &isophast_graph_1000, 16, 14, 1000);
        benchmark_one_to_many(&ch_graph, &ch_graph_1000, nullptr, &isophast_graph_1000, 20, 14, 1000);
        benchmark_one_to_many(&ch_graph, &ch_graph_1000, nullptr, &isophast_graph_1000, 18, 16, 1000);
    }
    {
        auto ch_graph_5000 = loadCHGraph2(GRAPH_DIR + "/" + GRAPH_NAME + "_ch_tiled_5000");
        std::cout << "finished loading ch-graph 5000..." << std::endl;
        auto grasp_graph_5000 = loadTiledGraph(GRAPH_DIR + "/" + GRAPH_NAME + "_grasp_5000");
        std::cout << "finished loading grasp-graph 5000..." << std::endl;
        auto isophast_graph_5000 = loadTiledGraph(GRAPH_DIR + "/" + GRAPH_NAME + "_isophast_5000");
        std::cout << "finished loading isophast-graph 5000..." << std::endl;
        benchmark_one_to_many(&ch_graph, &ch_graph_5000, &grasp_graph_5000, &isophast_graph_5000, 14, 14, 5000);
        benchmark_one_to_many(&ch_graph, &ch_graph_5000, &grasp_graph_5000, &isophast_graph_5000, 16, 14, 5000);
        benchmark_one_to_many(&ch_graph, &ch_graph_5000, &grasp_graph_5000, &isophast_graph_5000, 20, 14, 5000);
        benchmark_one_to_many(&ch_graph, &ch_graph_5000, &grasp_graph_5000, &isophast_graph_5000, 18, 16, 5000);
    }
    {
        auto ch_graph_10000 = loadCHGraph2(GRAPH_DIR + "/" + GRAPH_NAME + "_ch_tiled_10000");
        std::cout << "finished loading ch-graph 10000..." << std::endl;
        auto grasp_graph_10000 = loadTiledGraph(GRAPH_DIR + "/" + GRAPH_NAME + "_grasp_10000");
        std::cout << "finished loading grasp-graph 10000..." << std::endl;
        auto isophast_graph_10000 = loadTiledGraph(GRAPH_DIR + "/" + GRAPH_NAME + "_isophast_10000");
        std::cout << "finished loading isophast-graph 10000..." << std::endl;
        benchmark_one_to_many(&ch_graph, &ch_graph_10000, &grasp_graph_10000, &isophast_graph_10000, 14, 14, 10000);
        benchmark_one_to_many(&ch_graph, &ch_graph_10000, &grasp_graph_10000, &isophast_graph_10000, 16, 14, 10000);
        benchmark_one_to_many(&ch_graph, &ch_graph_10000, &grasp_graph_10000, &isophast_graph_10000, 20, 14, 10000);
        benchmark_one_to_many(&ch_graph, &ch_graph_10000, &grasp_graph_10000, &isophast_graph_10000, 18, 16, 10000);
    }
}
