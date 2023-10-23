#pragma once

#include <time.h>
#include <algorithm>
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
#include "../util.h"
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

void benchmark_one_to_many(ICHGraph* ch_graph, CHGraph2* ch_graph_2, ITiledGraph* grasp_graph, ITiledGraph* isophast_graph, int b, int t, int tilecount)
{
    std::cout << "start one-to-many benchmark:" << std::endl;
    // init results
    Results results("One-to-Many");

    std::cout << "create benchmark data..." << std::endl;

    // create random benchmark data
    const int N = 5;
    const int N2 = 3;
    const int N3 = 3;
    const int RANGE = 2400;
    const int B = std::pow(2, b);
    const int T = std::pow(2, t);
    std::vector<std::vector<int>> start_nodes;
    std::vector<std::vector<std::vector<bool>>> target_nodes;
    auto START_GRAPH = ch_graph;
    RNG rng(0, START_GRAPH->nodeCount() - 1, RANGE);
    for (int i = 0; i < N; i++) {
        // get center for ball query
        int center = rng.rand();
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
        RNG rng_starts(0, nodes.size() - 1, time(0));
        for (int j = 0; j < N3; j++) {
            int start = rng_starts.rand();
            starts.push_back(start);
        }
        start_nodes.push_back(std::move(starts));
    }

    std::cout << "start benchmarks..." << std::endl;

    // compute benchmarks for every value
    for (int i = 0; i < N; i++) {
        // create benchmark instance
        auto bench = nanobench::Bench();

        // get current test value
        std::vector<int>& starts_list = start_nodes[i];
        std::vector<std::vector<bool>>& targets_list = target_nodes[i];

        std::vector<bool> targets_ch(ch_graph->nodeCount());
        std::vector<bool> targets_ch_2;
        if (ch_graph_2 != nullptr) {
            targets_ch_2 = std::vector<bool>(ch_graph_2->nodeCount());
        }
        std::vector<bool> targets_grasp;
        if (grasp_graph != nullptr) {
            targets_grasp = std::vector<bool>(grasp_graph->nodeCount());
        }
        std::vector<bool> targets_isophast;
        if (isophast_graph != nullptr) {
            targets_isophast = std::vector<bool>(isophast_graph->nodeCount());
        }
        std::vector<bool> found_tiles(tilecount);
        std::vector<bool> active_tiles(tilecount);
        for (int k = 0; k < N2; k++) {
            // get targets for different graphs
            targets_ch = targets_list[k];
            if (ch_graph_2 != nullptr) {
                convert_targets(START_GRAPH, ch_graph_2, targets_list[k], targets_ch_2);
            }
            if (grasp_graph != nullptr) {
                convert_targets(START_GRAPH, grasp_graph, targets_list[k], targets_grasp);
            }
            if (isophast_graph != nullptr) {
                convert_targets(START_GRAPH, isophast_graph, targets_list[k], targets_isophast);
            }

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
            results.addResult(i, "RPHAST-Preprocessing (Space)", down_edges_subset.size());

            // benchmark range-rphast preprocessing
            std::vector<CHEdge> down_edges_subset_range;
            bench.run("RangeRPHAST-Preprocessing", [&] {
                std::priority_queue<pq_item> node_queue;
                for (int i = 0; i < ch_graph->nodeCount(); i++) {
                    if (targets_ch[i]) {
                        node_queue.push({i, 0});
                    }
                }
                std::vector<CHEdge> subset = preprocessRangeRPHAST(ch_graph, std::move(node_queue), RANGE);
                down_edges_subset_range = std::move(subset);
            });
            results.addResult(i, "RangeRPHAST-Preprocessing (Space)", down_edges_subset_range.size());

            std::vector<CHEdge4> down_edges_subset_gs;
            std::vector<CHEdge4> down_edges_subset_gs_range;
            if (ch_graph_2 != nullptr) {
                // benchmark gs-rphast preprocessing
                bench.run("GSRPHAST-Preprocessing", [&] {
                    std::queue<int> node_queue;
                    for (int i = 0; i < ch_graph_2->nodeCount(); i++) {
                        if (targets_ch_2[i]) {
                            node_queue.push(i);
                        }
                    }
                    std::vector<CHEdge4> subset = preprocessGSRPHAST(ch_graph_2, std::move(node_queue));
                    down_edges_subset_gs = std::move(subset);
                });
                results.addResult(i, "GSRPHAST-Preprocessing (Space)", down_edges_subset_gs.size());
                // benchmark range-gs-rphast preprocessing
                bench.run("RangeGSRPHAST-Preprocessing", [&] {
                    std::priority_queue<pq_item> node_queue;
                    for (int i = 0; i < ch_graph_2->nodeCount(); i++) {
                        if (targets_ch_2[i]) {
                            node_queue.push({i, 0});
                        }
                    }
                    std::vector<CHEdge4> subset = preprocessRangeGSRPHAST(ch_graph_2, std::move(node_queue), RANGE);
                    down_edges_subset_gs_range = std::move(subset);
                });
                results.addResult(i, "RangeGSRPHAST-Preprocessing (Space)", down_edges_subset_gs_range.size());
            }

            for (int j = 0; j < N3; j++) {
                // get start for different graphs
                int start = starts_list[j];
                int start_ch = start;
                int start_ch_2;
                if (ch_graph_2 != nullptr) {
                    start_ch_2 = convert_start(START_GRAPH, ch_graph_2, start);
                }
                int start_grasp;
                if (grasp_graph != nullptr) {
                    start_grasp = convert_start(START_GRAPH, grasp_graph, start);
                }
                int start_isophast;
                if (isophast_graph != nullptr) {
                    start_isophast = convert_start(START_GRAPH, isophast_graph, start);
                }

                DistFlagArray flags(START_GRAPH->nodeCount());

                // benchmark range-dijkstra
                bench.run("Range-Dijkstra", [&] {
                    flags.soft_reset();
                    calcRangeDijkstra(ch_graph, start_ch, flags, RANGE);
                });

                // benchmark phast
                bench.run("PHAST", [&] {
                    flags.soft_reset();
                    calcPHAST(ch_graph, start_ch, flags);
                });

                // benchmark rphast
                bench.run("RPHAST", [&] {
                    flags.soft_reset();
                    calcRPHAST(ch_graph, start_ch, flags, down_edges_subset);
                });

                // benchmark range-phast
                bench.run("RangePHAST", [&] {
                    flags.soft_reset();
                    calcRangePHAST(ch_graph, start_ch, flags, RANGE);
                });

                // benchmark range-rphast with normal preprocessing
                bench.run("RangeRPHAST (queue)", [&] {
                    flags.soft_reset();
                    calcRangeRPHAST(ch_graph, start_ch, flags, RANGE, down_edges_subset);
                });

                // benchmark range-rphast with range preprocessing
                bench.run("RangeRPHAST (prio-queue)", [&] {
                    flags.soft_reset();
                    calcRangeRPHAST(ch_graph, start_ch, flags, RANGE, down_edges_subset_range);
                });

                if (ch_graph_2 != nullptr) {
                    int tile_count = ch_graph_2->tileCount();
                    for (int i = 0; i < tile_count; i++) {
                        active_tiles[i] = false;
                    }
                    for (int i = 0; i < ch_graph_2->nodeCount(); i++) {
                        if (targets_ch_2[i]) {
                            short tile = ch_graph_2->getNodeTile(i);
                            active_tiles[tile] = true;
                        }
                    }
                    // benchmark GS+PHAST
                    bench.run("GSPHAST", [&] {
                        for (int i = 0; i < tile_count; i++) {
                            found_tiles[i] = false;
                        }
                        flags.soft_reset();
                        calcGSPHAST(ch_graph_2, start_ch_2, flags, RANGE, active_tiles, found_tiles);
                    });

                    // benchmark GS+RPHAST
                    bench.run("GSRPHAST", [&] {
                        for (int i = 0; i < tile_count; i++) {
                            found_tiles[i] = false;
                        }
                        flags.soft_reset();
                        calcGSRPHAST(ch_graph_2, start_ch_2, flags, RANGE, down_edges_subset_gs, active_tiles, found_tiles);
                    });

                    // benchmark GS+RPHAST (with priotity queue)
                    bench.run("GSRPHAST (prio)", [&] {
                        for (int i = 0; i < tile_count; i++) {
                            found_tiles[i] = false;
                        }
                        flags.soft_reset();
                        calcGSRPHAST(ch_graph_2, start_ch_2, flags, RANGE, down_edges_subset_gs_range, active_tiles, found_tiles);
                    });
                }

                if (grasp_graph != nullptr) {
                    // benchmark GRASP
                    int tile_count = grasp_graph->tileCount();
                    for (int i = 0; i < tile_count; i++) {
                        active_tiles[i] = false;
                    }
                    for (int i = 0; i < grasp_graph->nodeCount(); i++) {
                        if (targets_grasp[i]) {
                            short tile = grasp_graph->getNodeTile(i);
                            active_tiles[tile] = true;
                        }
                    }
                    bench.run("GRASP", [&] {
                        for (int i = 0; i < tile_count; i++) {
                            found_tiles[i] = false;
                        }
                        flags.soft_reset();
                        calcGRASP(grasp_graph, start_grasp, flags, RANGE, active_tiles, found_tiles);
                    });
                }

                if (isophast_graph != nullptr) {
                    // benchmark isoPHAST
                    int tile_count = isophast_graph->tileCount();
                    for (int i = 0; i < tile_count; i++) {
                        active_tiles[i] = false;
                    }
                    for (int i = 0; i < ch_graph->nodeCount(); i++) {
                        if (targets_isophast[i]) {
                            short tile = isophast_graph->getNodeTile(i);
                            active_tiles[tile] = true;
                        }
                    }
                    bench.run("isoPHAST", [&] {
                        for (int i = 0; i < tile_count; i++) {
                            found_tiles[i] = false;
                        }
                        flags.soft_reset();
                        calcGRASP(isophast_graph, start_isophast, flags, RANGE, active_tiles, found_tiles);
                    });
                }
            }
        }
        // get times from benchmark results
        auto& results_list = bench.results();
        for (int l = 0; l < results_list.size(); l++) {
            auto result = results_list[l];
            auto name = result.config().mBenchmarkName;
            double time = result.average(nanobench::Result::Measure::elapsed);
            results.addResult(i, name, time * 1000000);
        }
    }

    // write results to file
    std::cout << "start writing results..." << std::endl;
    write_results("results_one_to_many_" + std::to_string(tilecount) + "_" + std::to_string(b) + "_" + std::to_string(t) + ".csv", results);

    std::cout << "finished successfully!" << std::endl;
}