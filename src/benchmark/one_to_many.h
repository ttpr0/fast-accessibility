#pragma once

#include <time.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
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

int convert_start(IGraph* base_graph, IGraph* this_graph, int start)
{
    auto& index = this_graph->getIndex();
    int this_start = index.getClosestNode(base_graph->getNodeGeom(start));
    return this_start;
}

void convert_targets(IGraph* base_graph, IGraph* this_graph, std::vector<bool>& base_targets, std::vector<bool>& this_targets)
{
    auto& index = this_graph->getIndex();
    for (int i = 0; i < this_graph->nodeCount(); i++) {
        this_targets[i] = false;
    }
    for (int i = 0; i < base_graph->nodeCount(); i++) {
        if (base_targets[i]) {
            int node = index.getClosestNode(base_graph->getNodeGeom(i));
            if (node == -1) {
                continue;
            }
            this_targets[node] = true;
        }
    }
}

void benchmark_one_to_many(ICHGraph* ch_graph, CHGraph2* ch_graph_2, ITiledGraph* tiled_graph_1, ITiledGraph* tiled_graph_2)
{
    std::cout << "start one-to-many benchmark:" << std::endl;
    // init results
    std::vector<std::tuple<int, std::vector<int>>> results;
    std::vector<std::string> headers = {"Iteration",
                                        "RPHAST-Preprocessing",
                                        "RangeRPHAST-Preprocessing",
                                        "GSRPHAST-Preprocessing",
                                        "RangeDijkstra",
                                        "PHAST",
                                        "RPHAST",
                                        "RangePHAST",
                                        "RangeRPHAST",
                                        "RangeRPHAST2",
                                        "GSPHAST",
                                        "GSRPHAST",
                                        "GRASP",
                                        "isoPHAST"};
    const int BENCH_COUNT = 13;

    std::cout << "create benchmark data..." << std::endl;

    // create random benchmark data
    const int N = 5;
    const int N2 = 3;
    const int N3 = 3;
    const int RANGE = 2400;
    const int B = std::pow(2, 14);
    const int T = std::pow(2, 14);
    std::vector<std::vector<int>> start_nodes;
    std::vector<std::vector<std::vector<bool>>> target_nodes;
    auto START_GRAPH = ch_graph;
    srand(time(0));
    for (int i = 0; i < N; i++) {
        // get center for ball query
        int center = rand() % START_GRAPH->nodeCount();
        // compute ball query
        auto flags = DistFlagArray(START_GRAPH->nodeCount());
        calcCountDijkstra(START_GRAPH, center, flags, B);
        std::vector<int> nodes;
        for (int j = 0; j < START_GRAPH->nodeCount(); j++) {
            if (flags[j].dist < 10000000) {
                nodes.push_back(j);
            }
        }

        std::vector<std::vector<bool>> targets;
        for (int j = 0; j < N2; j++) {
            std::vector<bool> is_target(START_GRAPH->nodeCount());
            for (int k = 0; k < START_GRAPH->nodeCount(); k++) {
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
        std::vector<bool> targets_ch(ch_graph->nodeCount());
        std::vector<bool> targets_ch_2(ch_graph_2->nodeCount());
        std::vector<bool> targets_tiled_1(tiled_graph_1->nodeCount());
        std::vector<bool> targets_tiled_2(tiled_graph_2->nodeCount());
        for (int k = 0; k < N2; k++) {
            // get targets for different graphs
            targets_ch = targets_list[k];
            convert_targets(START_GRAPH, ch_graph_2, targets_list[k], targets_ch_2);
            convert_targets(START_GRAPH, tiled_graph_1, targets_list[k], targets_tiled_1);
            convert_targets(START_GRAPH, tiled_graph_2, targets_list[k], targets_tiled_2);
            for (int j = 0; j < N3; j++) {
                // get start for different graphs
                int start = starts_list[j];
                int start_ch = start;
                int start_ch_2 = convert_start(START_GRAPH, ch_graph_2, start);
                int start_tiled_1 = convert_start(START_GRAPH, tiled_graph_1, start);
                int start_tiled_2 = convert_start(START_GRAPH, tiled_graph_2, start);

                // create benchmark instance
                auto bench = nanobench::Bench();

                DistFlagArray flags(START_GRAPH->nodeCount());

                // benchmark rphast preprocessing
                std::vector<CHEdge> down_edges_subset;
                bench.run("RPHAST-Preprocessing", [&] {
                    std::queue<int> node_queue;
                    for (int i = 0; i < ch_graph->nodeCount(); i++) {
                        if (targets_ch[i]) {
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
                        if (targets_ch[i]) {
                            node_queue.push({i, 0});
                        }
                    }
                    std::vector<CHEdge> subset = preprocessRangeRPHAST(ch_graph, std::move(node_queue), RANGE);
                    down_edges_subset_2 = std::move(subset);
                });

                // benchmark range-rphast2 preprocessing
                std::vector<CHEdge4> down_edges_subset_3;
                bench.run("GSRPHAST-Preprocessing", [&] {
                    std::queue<int> node_queue;
                    for (int i = 0; i < ch_graph->nodeCount(); i++) {
                        if (targets_ch_2[i]) {
                            node_queue.push(i);
                        }
                    }
                    std::vector<CHEdge4> subset = preprocessGSRPHAST(ch_graph_2, std::move(node_queue));
                    down_edges_subset_3 = std::move(subset);
                });

                // benchmark range-dijkstra
                bench.run("Range-Dijkstra", [&] {
                    flags.soft_reset();
                    calcRangeDijkstra(ch_graph, start_ch, flags, RANGE);
                });

                // benchmark phast
                bench.run("PHAST", [&] {
                    flags.soft_reset();
                    calcPHAST2(ch_graph, start_ch, flags);
                });

                // benchmark rphast
                bench.run("RPHAST", [&] {
                    flags.soft_reset();
                    calcRPHAST(ch_graph, start_ch, flags, down_edges_subset);
                });

                // benchmark range-phast
                bench.run("RangePHAST", [&] {
                    flags.soft_reset();
                    calcRangePHAST5(ch_graph, start_ch, flags, RANGE);
                });

                // benchmark range-rphast with normal preprocessing
                bench.run("RangeRPHAST (queue)", [&] {
                    flags.soft_reset();
                    calcRangeRPHAST(ch_graph, start_ch, flags, RANGE, down_edges_subset);
                });

                // benchmark range-rphast with range preprocessing
                bench.run("RangeRPHAST (prio-queue)", [&] {
                    flags.soft_reset();
                    calcRangeRPHAST(ch_graph, start_ch, flags, RANGE, down_edges_subset_2);
                });

                // benchmark GS+PHAST
                bench.run("GSPHAST", [&] {
                    flags.soft_reset();
                    calcGSPHAST(ch_graph_2, start_ch_2, flags, RANGE);
                });

                // benchmark GS+RPHAST
                bench.run("GSRPHAST", [&] {
                    flags.soft_reset();
                    calcGSRPHAST(ch_graph_2, start_ch_2, flags, RANGE, down_edges_subset_3);
                });

                // GRASP-Preprocessing
                int tile_count = tiled_graph_1->tileCount();
                std::vector<bool> found_tiles(tile_count);
                std::vector<bool> active_tiles(tile_count);
                for (int i = 0; i < tile_count; i++) {
                    active_tiles[i] = false;
                }
                for (int i = 0; i < ch_graph->nodeCount(); i++) {
                    if (targets_tiled_1[i]) {
                        short tile = tiled_graph_1->getNodeTile(i);
                        active_tiles[tile] = true;
                    }
                }

                // benchmark GRASP
                bench.run("GRASP", [&] {
                    for (int i = 0; i < tile_count; i++) {
                        found_tiles[i] = false;
                    }
                    flags.soft_reset();
                    calcGRASP(tiled_graph_1, start, flags, RANGE, active_tiles, found_tiles);
                });

                // isoPHAST-Preprocessing
                int tile_count_2 = tiled_graph_2->tileCount();
                std::vector<bool> found_tiles_2(tile_count_2);
                std::vector<bool> active_tiles_2(tile_count_2);
                for (int i = 0; i < tile_count_2; i++) {
                    active_tiles_2[i] = false;
                }
                for (int i = 0; i < tiled_graph_2->nodeCount(); i++) {
                    if (targets_tiled_2[i]) {
                        short tile = tiled_graph_2->getNodeTile(i);
                        active_tiles_2[tile] = true;
                    }
                }

                // benchmark isoPHAST
                bench.run("isoPHAST", [&] {
                    for (int i = 0; i < tile_count_2; i++) {
                        found_tiles_2[i] = false;
                    }
                    flags.soft_reset();
                    calcGRASP(tiled_graph_2, start_tiled_2, flags, RANGE, active_tiles_2, found_tiles_2);
                });

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
    write_results("results_one_to_many.csv", results, headers);

    std::cout << "finished successfully!" << std::endl;
}
