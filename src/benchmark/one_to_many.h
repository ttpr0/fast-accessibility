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

void benchmark_one_to_many(ICHGraph* ch_graph, CHGraph2* ch_graph_1000, CHGraph2* ch_graph_5000, CHGraph2* ch_graph_10000, ITiledGraph* grasp_graph_1000,
                           ITiledGraph* grasp_graph_5000, ITiledGraph* grasp_graph_10000, ITiledGraph* isophast_graph_1000, ITiledGraph* isophast_graph_5000,
                           ITiledGraph* isophast_graph_10000)
{
    std::cout << "start one-to-many benchmark:" << std::endl;
    // init results
    std::vector<std::tuple<int, std::vector<int>>> results;
    std::vector<std::string> benchmarks = {};

    std::cout << "create benchmark data..." << std::endl;

    // create random benchmark data
    const int N = 1;
    const int N2 = 1;
    const int N3 = 1;
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
        // create benchmark instance
        auto bench = nanobench::Bench();

        // get current test value
        std::vector<int>& starts_list = start_nodes[i];
        std::vector<std::vector<bool>>& targets_list = target_nodes[i];

        std::vector<bool> targets_ch(ch_graph->nodeCount());
        std::vector<bool> targets_ch_1000(ch_graph_1000->nodeCount());
        std::vector<bool> targets_ch_5000(ch_graph_5000->nodeCount());
        std::vector<bool> targets_ch_10000(ch_graph_10000->nodeCount());
        std::vector<bool> targets_grasp_1000(grasp_graph_1000->nodeCount());
        std::vector<bool> targets_grasp_5000(grasp_graph_5000->nodeCount());
        std::vector<bool> targets_grasp_10000(grasp_graph_10000->nodeCount());
        std::vector<bool> targets_isophast_1000(isophast_graph_1000->nodeCount());
        std::vector<bool> targets_isophast_5000(isophast_graph_5000->nodeCount());
        std::vector<bool> targets_isophast_10000(isophast_graph_10000->nodeCount());
        std::vector<bool> found_tiles(100000);
        std::vector<bool> active_tiles(100000);
        for (int k = 0; k < N2; k++) {
            // get targets for different graphs
            targets_ch = targets_list[k];
            convert_targets(START_GRAPH, ch_graph_1000, targets_list[k], targets_ch_1000);
            convert_targets(START_GRAPH, ch_graph_5000, targets_list[k], targets_ch_5000);
            convert_targets(START_GRAPH, ch_graph_10000, targets_list[k], targets_ch_10000);
            convert_targets(START_GRAPH, grasp_graph_1000, targets_list[k], targets_grasp_1000);
            convert_targets(START_GRAPH, grasp_graph_5000, targets_list[k], targets_grasp_5000);
            convert_targets(START_GRAPH, grasp_graph_10000, targets_list[k], targets_grasp_10000);
            convert_targets(START_GRAPH, isophast_graph_1000, targets_list[k], targets_isophast_1000);
            convert_targets(START_GRAPH, isophast_graph_5000, targets_list[k], targets_isophast_5000);
            convert_targets(START_GRAPH, isophast_graph_10000, targets_list[k], targets_isophast_10000);

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

            // benchmark rphast+gs preprocessing
            std::vector<CHEdge4> down_edges_subset_gs_1000;
            bench.run("GSRPHAST-Preprocessing (1000)", [&] {
                std::queue<int> node_queue;
                for (int i = 0; i < ch_graph_1000->nodeCount(); i++) {
                    if (targets_ch_1000[i]) {
                        node_queue.push(i);
                    }
                }
                std::vector<CHEdge4> subset = preprocessGSRPHAST(ch_graph_1000, std::move(node_queue));
                down_edges_subset_gs_1000 = std::move(subset);
            });
            std::vector<CHEdge4> down_edges_subset_gs_5000;
            bench.run("GSRPHAST-Preprocessing (5000)", [&] {
                std::queue<int> node_queue;
                for (int i = 0; i < ch_graph_5000->nodeCount(); i++) {
                    if (targets_ch_5000[i]) {
                        node_queue.push(i);
                    }
                }
                std::vector<CHEdge4> subset = preprocessGSRPHAST(ch_graph_5000, std::move(node_queue));
                down_edges_subset_gs_5000 = std::move(subset);
            });
            std::vector<CHEdge4> down_edges_subset_gs_10000;
            bench.run("GSRPHAST-Preprocessing (10000)", [&] {
                std::queue<int> node_queue;
                for (int i = 0; i < ch_graph_10000->nodeCount(); i++) {
                    if (targets_ch_10000[i]) {
                        node_queue.push(i);
                    }
                }
                std::vector<CHEdge4> subset = preprocessGSRPHAST(ch_graph_10000, std::move(node_queue));
                down_edges_subset_gs_10000 = std::move(subset);
            });

            // benchmark range-rphast+gs preprocessing
            std::vector<CHEdge4> down_edges_subset_gs_1000_range;
            bench.run("RangeGSRPHAST-Preprocessing (1000)", [&] {
                std::priority_queue<pq_item> node_queue;
                for (int i = 0; i < ch_graph_1000->nodeCount(); i++) {
                    if (targets_ch_1000[i]) {
                        node_queue.push({i, 0});
                    }
                }
                std::vector<CHEdge4> subset = preprocessRangeGSRPHAST(ch_graph_1000, std::move(node_queue), RANGE);
                down_edges_subset_gs_1000_range = std::move(subset);
            });
            std::vector<CHEdge4> down_edges_subset_gs_5000_range;
            bench.run("RangeGSRPHAST-Preprocessing (5000)", [&] {
                std::priority_queue<pq_item> node_queue;
                for (int i = 0; i < ch_graph_5000->nodeCount(); i++) {
                    if (targets_ch_5000[i]) {
                        node_queue.push({i, 0});
                    }
                }
                std::vector<CHEdge4> subset = preprocessRangeGSRPHAST(ch_graph_5000, std::move(node_queue), RANGE);
                down_edges_subset_gs_5000_range = std::move(subset);
            });
            std::vector<CHEdge4> down_edges_subset_gs_10000_range;
            bench.run("RangeGSRPHAST-Preprocessing (10000)", [&] {
                std::priority_queue<pq_item> node_queue;
                for (int i = 0; i < ch_graph_10000->nodeCount(); i++) {
                    if (targets_ch_10000[i]) {
                        node_queue.push({i, 0});
                    }
                }
                std::vector<CHEdge4> subset = preprocessRangeGSRPHAST(ch_graph_10000, std::move(node_queue), RANGE);
                down_edges_subset_gs_10000_range = std::move(subset);
            });

            for (int j = 0; j < N3; j++) {
                // get start for different graphs
                int start = starts_list[j];
                int start_ch = start;
                int start_ch_1000 = convert_start(START_GRAPH, ch_graph_1000, start);
                int start_ch_5000 = convert_start(START_GRAPH, ch_graph_5000, start);
                int start_ch_10000 = convert_start(START_GRAPH, ch_graph_10000, start);
                int start_grasp_1000 = convert_start(START_GRAPH, grasp_graph_1000, start);
                int start_grasp_5000 = convert_start(START_GRAPH, grasp_graph_5000, start);
                int start_grasp_10000 = convert_start(START_GRAPH, grasp_graph_10000, start);
                int start_isophast_1000 = convert_start(START_GRAPH, isophast_graph_1000, start);
                int start_isophast_5000 = convert_start(START_GRAPH, isophast_graph_5000, start);
                int start_isophast_10000 = convert_start(START_GRAPH, isophast_graph_10000, start);

                DistFlagArray flags(START_GRAPH->nodeCount());

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
                    calcRangeRPHAST(ch_graph, start_ch, flags, RANGE, down_edges_subset_range);
                });

                // benchmark GS+PHAST
                bench.run("GSPHAST (1000)", [&] {
                    flags.soft_reset();
                    calcGSPHAST(ch_graph_1000, start_ch_1000, flags, RANGE);
                });
                bench.run("GSPHAST (5000)", [&] {
                    flags.soft_reset();
                    calcGSPHAST(ch_graph_5000, start_ch_5000, flags, RANGE);
                });
                bench.run("GSPHAST (10000)", [&] {
                    flags.soft_reset();
                    calcGSPHAST(ch_graph_10000, start_ch_10000, flags, RANGE);
                });

                // benchmark GS+RPHAST
                bench.run("GSRPHAST (1000)", [&] {
                    flags.soft_reset();
                    calcGSRPHAST(ch_graph_1000, start_ch_1000, flags, RANGE, down_edges_subset_gs_1000);
                });
                bench.run("GSRPHAST (5000)", [&] {
                    flags.soft_reset();
                    calcGSRPHAST(ch_graph_5000, start_ch_5000, flags, RANGE, down_edges_subset_gs_5000);
                });
                bench.run("GSRPHAST (10000)", [&] {
                    flags.soft_reset();
                    calcGSRPHAST(ch_graph_10000, start_ch_10000, flags, RANGE, down_edges_subset_gs_10000);
                });

                // benchmark GS+RPHAST (with priotity queue)
                bench.run("GSRPHAST (1000, prio)", [&] {
                    flags.soft_reset();
                    calcGSRPHAST(ch_graph_1000, start_ch_1000, flags, RANGE, down_edges_subset_gs_1000_range);
                });
                bench.run("GSRPHAST (5000, prio)", [&] {
                    flags.soft_reset();
                    calcGSRPHAST(ch_graph_5000, start_ch_5000, flags, RANGE, down_edges_subset_gs_5000_range);
                });
                bench.run("GSRPHAST (10000, prio)", [&] {
                    flags.soft_reset();
                    calcGSRPHAST(ch_graph_10000, start_ch_10000, flags, RANGE, down_edges_subset_gs_10000_range);
                });

                // benchmark GRASP
                int tile_count = grasp_graph_1000->tileCount();
                for (int i = 0; i < tile_count; i++) {
                    active_tiles[i] = false;
                }
                for (int i = 0; i < ch_graph->nodeCount(); i++) {
                    if (targets_grasp_1000[i]) {
                        short tile = grasp_graph_1000->getNodeTile(i);
                        active_tiles[tile] = true;
                    }
                }
                bench.run("GRASP (1000)", [&] {
                    for (int i = 0; i < tile_count; i++) {
                        found_tiles[i] = false;
                    }
                    flags.soft_reset();
                    calcGRASP(grasp_graph_1000, start_grasp_1000, flags, RANGE, active_tiles, found_tiles);
                });
                tile_count = grasp_graph_5000->tileCount();
                for (int i = 0; i < tile_count; i++) {
                    active_tiles[i] = false;
                }
                for (int i = 0; i < ch_graph->nodeCount(); i++) {
                    if (targets_grasp_5000[i]) {
                        short tile = grasp_graph_5000->getNodeTile(i);
                        active_tiles[tile] = true;
                    }
                }
                bench.run("GRASP (5000)", [&] {
                    for (int i = 0; i < tile_count; i++) {
                        found_tiles[i] = false;
                    }
                    flags.soft_reset();
                    calcGRASP(grasp_graph_5000, start_grasp_5000, flags, RANGE, active_tiles, found_tiles);
                });
                tile_count = grasp_graph_10000->tileCount();
                for (int i = 0; i < tile_count; i++) {
                    active_tiles[i] = false;
                }
                for (int i = 0; i < ch_graph->nodeCount(); i++) {
                    if (targets_grasp_10000[i]) {
                        short tile = grasp_graph_10000->getNodeTile(i);
                        active_tiles[tile] = true;
                    }
                }
                bench.run("GRASP (10000)", [&] {
                    for (int i = 0; i < tile_count; i++) {
                        found_tiles[i] = false;
                    }
                    flags.soft_reset();
                    calcGRASP(grasp_graph_10000, start_grasp_10000, flags, RANGE, active_tiles, found_tiles);
                });

                // benchmark isoPHAST
                tile_count = isophast_graph_1000->tileCount();
                for (int i = 0; i < tile_count; i++) {
                    active_tiles[i] = false;
                }
                for (int i = 0; i < ch_graph->nodeCount(); i++) {
                    if (targets_isophast_1000[i]) {
                        short tile = isophast_graph_1000->getNodeTile(i);
                        active_tiles[tile] = true;
                    }
                }
                bench.run("isoPHAST (1000)", [&] {
                    for (int i = 0; i < tile_count; i++) {
                        found_tiles[i] = false;
                    }
                    flags.soft_reset();
                    calcGRASP(isophast_graph_1000, start_isophast_1000, flags, RANGE, active_tiles, found_tiles);
                });
                tile_count = isophast_graph_5000->tileCount();
                for (int i = 0; i < tile_count; i++) {
                    active_tiles[i] = false;
                }
                for (int i = 0; i < ch_graph->nodeCount(); i++) {
                    if (targets_isophast_5000[i]) {
                        short tile = isophast_graph_5000->getNodeTile(i);
                        active_tiles[tile] = true;
                    }
                }
                bench.run("isoPHAST (5000)", [&] {
                    for (int i = 0; i < tile_count; i++) {
                        found_tiles[i] = false;
                    }
                    flags.soft_reset();
                    calcGRASP(isophast_graph_5000, start_isophast_5000, flags, RANGE, active_tiles, found_tiles);
                });
                tile_count = isophast_graph_10000->tileCount();
                for (int i = 0; i < tile_count; i++) {
                    active_tiles[i] = false;
                }
                for (int i = 0; i < ch_graph->nodeCount(); i++) {
                    if (targets_isophast_10000[i]) {
                        short tile = isophast_graph_10000->getNodeTile(i);
                        active_tiles[tile] = true;
                    }
                }
                bench.run("isoPHAST (10000)", [&] {
                    for (int i = 0; i < tile_count; i++) {
                        found_tiles[i] = false;
                    }
                    flags.soft_reset();
                    calcGRASP(isophast_graph_10000, start_isophast_10000, flags, RANGE, active_tiles, found_tiles);
                });
            }
        }
        // get times
        std::unordered_map<std::string, std::tuple<double, int>> times;
        auto& results_list = bench.results();
        for (int l = 0; l < results_list.size(); l++) {
            auto result = results_list[l];
            auto name = result.config().mBenchmarkName;
            // add benchmark names to benchmarks list
            if (std::find(benchmarks.begin(), benchmarks.end(), name) == benchmarks.end()) {
                benchmarks.push_back(name);
            }
            double time = result.average(nanobench::Result::Measure::elapsed);
            if (times.contains(name)) {
                auto [t, c] = times[name];
                t += time;
                c += 1;
                times[name] = std::make_tuple(t, c);
            } else {
                times[name] = std::make_tuple(time, 1);
            }
        }

        std::cout << "start writing results..." << std::endl;

        // gather results
        std::vector<int> result_times(benchmarks.size());
        for (int j = 0; j < benchmarks.size(); j++) {
            auto& bench_name = benchmarks[j];
            auto [time, count] = times[bench_name];
            result_times[j] = time * 1000000 / count;
        }
        results.push_back(make_tuple(i, result_times));
    }

    // write results to file
    std::vector<std::string> headers = {"Iteration"};
    for (int j = 0; j < benchmarks.size(); j++) {
        auto& bench_name = benchmarks[j];
        headers.push_back(bench_name);
    }
    write_results("results_one_to_many.csv", results, headers);

    std::cout << "finished successfully!" << std::endl;
}
