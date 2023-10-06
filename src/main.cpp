#include <time.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "algorithm/all_dijkstra.h"
#include "algorithm/phast.h"
#include "algorithm/range_phast.h"
#include "benchmark/benchmark.h"
#include "graph/graph.h"
#include "graph/loader.h"

using namespace std;

int main()
{
    // auto graph = loadGraph("./graphs/germany_ch");
    // auto ch_graph_1 = loadCHGraph("D:/Routing_Graphen/niedersachsen/niedersachsen_ch_reordered");
    // auto ch_graph_2 = loadCHGraph2("D:/Routing_Graphen/niedersachsen/niedersachsen_ch_tiled_1000");
    // auto tiled_graph_1 = loadTiledGraph("D:/Routing_Graphen/niedersachsen/niedersachsen_tiled_1000_grasp");
    // auto tiled_graph_2 = loadTiledGraph("D:/Routing_Graphen/niedersachsen/niedersachsen_tiled_1000_phast");

    cout << "start loading graphs..." << endl;
    // auto tiled_graph_1 = loadTiledGraph("D:/Routing_Graphen/germany/germany_tiled_10000_grasp");
    // cout << "finished loading tiled-graph..." << endl;
    auto ch_graph_1 = loadCHGraph("D:/Routing_Graphen/germany/germany_ch_reordered");
    cout << "finished loading ch-graph..." << endl;
    auto ch_graph_2 = loadCHGraph2("D:/Routing_Graphen/germany/germany_ch_tiled_10000");
    cout << "finished loading ch-graph 2..." << endl;
    auto tiled_graph_2 = loadTiledGraph("D:/Routing_Graphen/germany/germany_tiled_10000_phast");
    cout << "finished loading tiled-graph 2..." << endl;

    // benchmark_range_search(graph.get(), ch_graph.get());
    // benchmark_supply_count(graph.get(), ch_graph.get());
    // benchmark_one_to_many(ch_graph_1.get(), ch_graph_2.get(), tiled_graph_1.get(), tiled_graph_2.get());
    // benchmark_catchment_range(ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    // benchmark_supply_count(ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    // benchmark_demand_count(ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    // benchmark_study_area(ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());

    // benchmark hausarzt
    benchmark_2sfca_run("./data/physician/mittelbereiche/physicians_munster_hausarzt.txt", "./data/points/mittelbereiche/population_munster.txt", 1930,
                        "results_2sfca_munster_hausarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/mittelbereiche/physicians_westerstede_hausarzt.txt", "./data/points/mittelbereiche/population_westerstede.txt", 1930,
                        "results_2sfca_westerstede_hausarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/mittelbereiche/physicians_hildesheim_hausarzt.txt", "./data/points/mittelbereiche/population_hildesheim.txt", 1930,
                        "results_2sfca_hildesheim_hausarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/physicians_niedersachsen_hausarzt.txt", "./data/points/population_niedersachsen.txt", 1930, "results_2sfca_niedersachsen_hausarzt.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());

    // benchmark augenarzt
    benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_augenarzt.txt", "./data/points/landkreise/population_emden.txt", 2413, "results_2sfca_emden_augenarzt.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_augenarzt.txt", "./data/points/landkreise/population_braunschweig.txt", 2413,
                        "results_2sfca_braunschweig_augenarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_augenarzt.txt", "./data/points/landkreise/population_region_hannover.txt", 2413,
                        "results_2sfca_region_hannover_augenarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/physicians_niedersachsen_augenarzt.txt", "./data/points/population_niedersachsen.txt", 2413, "results_2sfca_niedersachsen_augenarzt.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());

    // benchmark frauenarzt
    benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_frauenarzt.txt", "./data/points/landkreise/population_emden.txt", 2550, "results_2sfca_emden_frauenarzt.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_frauenarzt.txt", "./data/points/landkreise/population_braunschweig.txt", 2550,
                        "results_2sfca_braunschweig_frauenarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_frauenarzt.txt", "./data/points/landkreise/population_region_hannover.txt", 2550,
                        "results_2sfca_region_hannover_frauenarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/physicians_niedersachsen_frauenarzt.txt", "./data/points/population_niedersachsen.txt", 2550,
                        "results_2sfca_niedersachsen_frauenarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());

    // benchmark hautarzt
    benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_hautarzt.txt", "./data/points/landkreise/population_emden.txt", 2802, "results_2sfca_emden_hautarzt.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_hautarzt.txt", "./data/points/landkreise/population_braunschweig.txt", 2802,
                        "results_2sfca_braunschweig_hautarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_hautarzt.txt", "./data/points/landkreise/population_region_hannover.txt", 2802,
                        "results_2sfca_region_hannover_hautarzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/physicians_niedersachsen_hautarzt.txt", "./data/points/population_niedersachsen.txt", 2802, "results_2sfca_niedersachsen_hautarzt.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());

    // benchmark hno_arzt
    benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_hno_arzt.txt", "./data/points/landkreise/population_emden.txt", 2477, "results_2sfca_emden_hno_arzt.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_hno_arzt.txt", "./data/points/landkreise/population_braunschweig.txt", 2477,
                        "results_2sfca_braunschweig_hno_arzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_hno_arzt.txt", "./data/points/landkreise/population_region_hannover.txt", 2477,
                        "results_2sfca_region_hannover_hno_arzt.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/physicians_niedersachsen_hno_arzt.txt", "./data/points/population_niedersachsen.txt", 2477, "results_2sfca_niedersachsen_hno_arzt.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());

    // benchmark orthopade
    benchmark_2sfca_run("./data/physician/landkreise/physicians_emden_orthopade.txt", "./data/points/landkreise/population_emden.txt", 3076, "results_2sfca_emden_orthopade.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_braunschweig_orthopade.txt", "./data/points/landkreise/population_braunschweig.txt", 3076,
                        "results_2sfca_braunschweig_orthopade.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/landkreise/physicians_region_hannover_orthopade.txt", "./data/points/landkreise/population_region_hannover.txt", 3076,
                        "results_2sfca_region_hannover_orthopade.csv", ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
    benchmark_2sfca_run("./data/physician/physicians_niedersachsen_orthopade.txt", "./data/points/population_niedersachsen.txt", 3076, "results_2sfca_niedersachsen_orthopade.csv",
                        ch_graph_1.get(), ch_graph_2.get(), tiled_graph_2.get());
}
