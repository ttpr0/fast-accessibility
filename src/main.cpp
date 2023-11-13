#include <time.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "./util.h"
#include "accessibility/batched_shortest_path.h"
#include "algorithm/all_dijkstra.h"
#include "algorithm/phast.h"
#include "algorithm/range_phast.h"
#include "benchmark/benchmark.h"
#include "graph/graph.h"
#include "graph/io/loader.h"
#include "graph/mod/id_mapping_mod.h"
#include "routing/one_to_many/dijkstra.h"

using namespace std;

int main()
{
    auto graph = loadCHGraph("D:/graphs/bench_ch");

    auto [dem_points, dem_weights] = read_points("./data/population_wittmund.txt");
    auto [sup_points, sup_weights] = read_points("./data/physicians_wittmund.txt");

    auto access_1 = calcDijkstra2SFCA(&graph, dem_points, dem_weights, sup_points, sup_weights, 1800);
    cout << access_1[0] << endl;

    auto access_2 = calc2SFCA<RangeDijkstra>(&graph, dem_points, dem_weights, sup_points, sup_weights, 1800);
    cout << access_2[0] << endl;
}
