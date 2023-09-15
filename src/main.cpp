#include <time.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "./benchmark.h"
#include "algorithm/all_dijkstra.h"
#include "algorithm/phast.h"
#include "algorithm/range_phast.h"
#include "benchmark.h"
#include "graph/graph.h"
#include "graph/loader.h"

using namespace std;

// #define PRINT_RESULT

int main()
{
    auto graph = loadGraph("./graphs/bench_ch");
    auto ch_graph = loadCHGraph("./graphs/bench_ch");

    const std::vector<CHEdge>& down_edges = ch_graph->getDownEdges(Direction::FORWARD);
    int length = down_edges.size();
    int count = 0;
    int sum = 0;
    for (int i = 0; i < length; ++i) {
        auto edge = down_edges[i];
        sum += edge.count;
        count += 1;
        if (count == 10000) {
            int avg = sum / count;
            if (avg == 2) {
                cout << i << endl;
            }
            sum = 0;
            count = 0;
        }
        i += edge.count - 1;
    }

    // benchmark_range_search(graph.get(), ch_graph.get());
    benchmark_supply_count(graph.get(), ch_graph.get());

    // auto [dem_points, dem_weights] = read_points("./data/population_wittmund.txt");
    // auto [sup_points, sup_weights] = read_points("./data/physicians_wittmund.txt");

    // calcDijkstra2SFCA(graph.get(), dem_points, dem_weights, sup_points, sup_weights, 1800);

    // int start = 100;
    // int range = 1800;
    // int n = 10023;

    // auto dist = calcAllDijkstra(ch_graph.get(), start, range);
    // cout << "Dijkstra: " << dist[n] << endl;
    // auto dist = calcPHAST(ch_graph.get(), start);
    // cout << "PHAST: " << dist[n] << endl;
    // auto dist = calcRangePHAST(ch_graph.get(), start, range);
    // cout << "RangePHAST: " << dist[n] << endl;
    // auto dist = calcRangePHAST2(ch_graph.get(), start, range);
    // cout << "RangePHAST2: " << dist[n] << endl;
    // auto dist = calcRangePHAST3(ch_graph.get(), start, range);
    // cout << "RangePHAST3: " << dist[n] << endl;

    // auto t1 = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < N; i++) {
    //     int start = start_nodes[i];
    //     calcAllDijkstra(graph.get(), start, range);
    // }
    // auto t2 = std::chrono::high_resolution_clock::now();
    // auto dt1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    // cout << "time: " << dt1.count() / N << endl;

    // t1 = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < N; i++) {
    //     int start = start_nodes[i];
    //     calcPHAST(ch_graph.get(), start, range);
    // }
    // t2 = std::chrono::high_resolution_clock::now();
    // auto dt2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    // cout << "time: " << dt2.count() / N << endl;

    // t1 = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < N; i++) {
    //     int start = start_nodes[i];
    //     calcPHAST2(ch_graph.get(), start, range);
    // }
    // t2 = std::chrono::high_resolution_clock::now();
    // auto dt3 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    // cout << "time: " << dt3.count() / N << endl;

    // t1 = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < N; i++) {
    //     int start = start_nodes[i];
    //     calcPHAST3(ch_graph.get(), start, range);
    // }
    // t2 = std::chrono::high_resolution_clock::now();
    // auto dt4 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    // cout << "time: " << dt4.count() / N << endl;
}
