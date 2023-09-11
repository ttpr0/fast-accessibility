#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "algorithm/all_dijkstra.h"
#include "algorithm/phast.h"
#include "algorithm/phast2.h"
#include "algorithm/phast3.h"
#include "graph/graph.h"
#include "graph/loader.h"

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

using namespace std;

// #define PRINT_RESULT

int main()
{
    auto graph = loadGraph("./graphs/bench_ch");
    auto ch_graph = loadCHGraph("./graphs/bench_ch");

    const int N = 1;
    std::vector<int> start_nodes;
    for (int i = 0; i < N; i++) {
        start_nodes.push_back(rand() % graph->nodeCount());
    }
    int range = 900;
#ifdef PRINT_RESULT
    int n = 10023;
#endif

    ankerl::nanobench::Bench().run("test 1", [&] {
        for (int i = 0; i < N; i++) {
            int start = start_nodes[i];
#ifdef PRINT_RESULT
            auto dist = calcAllDijkstra(graph.get(), start, range);
            cout << "Dijkstra: " << dist[n] << endl;
#else
            calcAllDijkstra(graph.get(), start, range);
#endif
        }
    });
    ankerl::nanobench::Bench().run("test 2", [&] {
        for (int i = 0; i < N; i++) {
            int start = start_nodes[i];
#ifdef PRINT_RESULT
            auto dist = calcPHAST(ch_graph.get(), start, range);
            cout << "PHAST: " << dist[n] << endl;
#else
            calcPHAST(ch_graph.get(), start, range);
#endif
        }
    });

    ankerl::nanobench::Bench().run("test 3", [&] {
        for (int i = 0; i < N; i++) {
            int start = start_nodes[i];
#ifdef PRINT_RESULT
            auto dist = calcPHAST2(ch_graph.get(), start, range);
            cout << "PHAST2: " << dist[n] << endl;
#else
            calcPHAST2(ch_graph.get(), start, range);
#endif
        }
    });
    ankerl::nanobench::Bench().run("test 4", [&] {
        for (int i = 0; i < N; i++) {
            int start = start_nodes[i];
#ifdef PRINT_RESULT
            auto dist = calcPHAST3(ch_graph.get(), start, range);
            cout << "PHAST3: " << dist[n] << endl;
#else
            calcPHAST3(ch_graph.get(), start, range);
#endif
        }
    });

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
