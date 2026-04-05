// -*- coding: utf-8 -*-
// ============================================================================
// Project: Nemertea
// File: proboscis.hpp
//
// Part of the Nemertea Project
// Territorial Expansion-Based Algorithm for the Hamiltonian Cycle Problem
//
// © 2021-Present Saulo Popov Zambiasi. All rights reserved.
// Registered at INPI [BR512025005332-0].
//
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.
// ============================================================================
// Description:
// Declaration of the Proboscis class, which implements the Nemertea BFS
// algorithm for finding the shortest path between two vertices in a graph.
// ============================================================================

#ifndef NBFS_HPP_
#define NBFS_HPP_

#include "graph.hpp"
#include <cstddef>
#include <cstdint>
#include <queue>

/**
 * Proboscis is a class that implements the Nemertea BFS algorithm for finding
 * the shortest path between two vertices in a graph. It uses a breadth-first
 * search approach to explore the graph and find the shortest path. The class
 * maintains a queue of leaves to explore and a generation counter to keep
 * track of the current search iteration. The Probe method is used to check
 * if a vertex is the target vertex, and the Retract method is used to
 * backtrack through the path once the target vertex is found.
 *
 * Attributes
 * ----------
 * - graph_     : The graph on which the algorithm operates.
 * - root_      : The starting vertex for the search.
 * - generation_ : The current generation ID for the search iteration.
 * - leaves_     : A queue of vertices to explore during the search.
 */
class Proboscis
{
private:
    Graph *graph_;                // Graph
    Vertex *root_;                // root vertex on the NBFS search
    uint64_t generation_;         // current id of NBFS search
    std::queue<Vertex *> leaves_; // leaves of the NBFS search

    /**
     * Probes the given vertex to check if it is the target vertex. If the
     * vertex is the target, it returns a pair containing the vertex and a
     * boolean value of true. If the vertex is not the target, it returns a
     * pair containing the vertex and a boolean value of false. The first
     * parameter indicates whether this is the first probe in the search.
     * @param vertex The vertex to probe.
     * @param target The target vertex to compare against.
     * @param first A boolean indicating if this is the first probe in the search.
     * @return A pair containing the probed vertex and a boolean indicating if it is the target.
     */
    std::pair<Vertex *, bool> Probe(Vertex *vertex, Vertex *target, bool first) const;

    /**
     * Retracts the path from the given vertex back to the root vertex,
     * counting the number of steps taken. This method is used to determine
     * the length of the path from the target vertex back to the root vertex
     * once the target vertex is found during the search. All vertex in the
     * way back to the root vertex will be set as CONQUERED.
     * @param vertex The vertex from which to start the retraction.
     * @return The number of steps taken to retract back to the root vertex.
     */
    size_t Retract(Vertex *vertex) const;

public:
    /**
     * Constructs a Proboscis object with the given graph. The root vertex is
     * initialized to nullptr, and the generation counter is initialized to 0.
     * @param graph The graph on which the algorithm will operate.
     */
    Proboscis(Graph *graph) : graph_(graph), root_(nullptr) {};

    /**
     * Executes the Nemertea BFS algorithm to find the shortest path from the
     * start vertex to the target vertex. The method returns the length of the
     * shortest path found. If no path is found, it returns 0.
     * @param startVertex The starting vertex for the search.
     * @param targetVertex The target vertex to find.
     * @return The length of the shortest path from startVertex to targetVertex, or 0 if no path is found.
     */
    size_t Evert(Vertex *startVertex, bool first, size_t depth);
};

#endif // NBFS_HPP_