// -*- coding: utf-8 -*-
// vertex.h
//
// Nemertea: A Territorial Expansion-Based Algorithm for the Hamiltonian
// Cycle Problem
//
// © 2021-Present Saulo Popov Zambiasi. All rights reserved.
// Since 10/10/2021.
// Registered at INPI (Brazil).
// Contact: saulopz@gmail.com
//
// This file is part of the Nemertea source code,
// implementing the Vertex class used in the NBFS algorithm.
//
// Description:
//
// Nemertea is an algorithm for solving the Hamiltonian cycle problem in
// graphs. It is a heuristic algorithm that uses territorial conquest as
// a strategy. The algorithm starts in a random closed region and adds
// new closed regions  * until all vertices are part of the boundaries of
// that territory. To add new regions, a vertex v is taken from the
// frontier and follows paths and external vertices of the frontier until
// it finds a vertex u that is a neighbor of v and is on the frontier,
// with no other vertices between them. The path taken from v to u through
// the external area is added as a new frontier and the boundary that was
// between v and u is undone. In this way, a new area is added to the
// territory. To find this new region, the algorithm uses a custom Breadth
// First Search.

/**
 * @section Performance and Memory Architecture
 * * The Nemertea algorithm utilizes raw pointers for graph traversal and
 * vertex manipulation. This design choice is deliberate and aims to:
 * * 1. Minimize Overhead: Avoiding the reference counting of std::shared_ptr
 * or the movement semantics of std::unique_ptr within the "hot path" of
 * the custom Breadth-First Search (NBFS), where performance is critical.
 * * 2. Ownership Separation: The Graph object (see graph.h) maintains total
 * ownership (RAII) of all vertices and edges. The Nemertea function
 * acts solely as a consumer/visitor. Since the graph structure remains
 * immutable throughout the expansion process, raw pointers provide the
 * most efficient access pattern without risking memory leaks or
 * dangling pointers.
 * * 3. Cache Friendliness: Direct pointer dereferencing facilitates compiler
 * optimizations and aligns with the high-performance requirements of
 * solving NP-complete problems like the Hamiltonian Cycle.
 */

#include "graph.hpp"
#include "nemertea.hpp"
#include <chrono>
#include <cxxopts.hpp>
#include <iostream>

int main(const int argc, char *argv[])
{
    // Parameters inicialization ------------------------------------

    cxxopts::Options options(                                      //
        "nemertea",                                                //
        "Nemertea: A Territorial Expansion-Based Algorithm "       //
        "for the Hamiltonian Cycle Problem");                      //
                                                                   //
    options.add_options()                                          //
        ("g,graph", "Graph file (.json)",                          //
         cxxopts::value<std::string>()->default_value(""))         //
        ("d,depth", "Max depth (between 3 and 20, default 5)",     //
         cxxopts::value<int>()->default_value("5"))                //
        ("t,type", "Operation type (cycle, path. Default: cycle)", //
         cxxopts::value<std::string>()->default_value("cycle"))    //
        ("h,help", "Show this help message");

    std::string fname = "";
    size_t max_depth = 5;
    std::string type = "cycle";

    try
    {
        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            std::cout << options.help() << "\n";
            return 0;
        }

        fname = result["graph"].as<std::string>();
        max_depth = static_cast<size_t>(result["depth"].as<int>());
        type = result["type"].as<std::string>();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error interpreting parameters: " << e.what() << "\n";
        std::cerr << "Use --help to see the available options.\n";
        return 1;
    }

    if (fname == "")
    {
        std::cout << "You need enter with a graph file.\n" << std::endl;
        std::cout << options.help() << "\n";
        return 1;
    }

    // Running program -------------------------------------------

    auto graph = std::make_unique<Graph>(fname);
    graph->Load();
    const size_t vertex_count = graph->GetVertexCount();

    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    const auto initial = high_resolution_clock::now();

    auto nemertea = Nemertea(graph.get());
    const size_t path_count = nemertea.Walk(max_depth, type == "cycle");

    const auto duration = duration_cast<microseconds>( //
        high_resolution_clock::now() - initial         //
    );

    // Printing results --------------------------------------------

    std::cout << "Nemertea execution " << duration.count() //
              << " microseconds. Found " << path_count     //
              << " of " << vertex_count << " vertices =>"; //

    const bool solved = path_count == vertex_count;
    if (solved)
        std::cout << " SOLVED." << std::endl;
    else
        std::cout << " INCONCLUSIVE." << std::endl;

    graph->Save(solved);

    return 0;
}
