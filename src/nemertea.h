// -*- coding: utf-8 -*-
// nemertea.h
//
// Nemertea: A Territorial Expansion-Based Algorithm
// for the Hamiltonian Cycle Problem
//
// © 2025 Saulo Popov Zambiasi. All rights reserved.
// Registered at INPI (Brazil).
// Contact: saulopz@gmail.com
//
// This file is part of the Nemertea source code,
// implementing the Vertex class used in the NBFS algorithm.
//

#ifndef NEMERTEA_H_
#define NEMERTEA_H_

#include "graph.h"

/**
 * @brief Executes the Nemertea algorithm on the provided graph.
 *
 * @param graph Pointer to the graph on which to run the algorithm.
 * @param depth The depth parameter for the NBFS algorithm.
 * @param cycle Boolean indicating whether to search for a cycle (true) or a path (false).
 * @return The number of vertices included in the path or cycle found.
 */
size_t Nemertea(Graph *graph, size_t depth, bool cycle);

/**
 * @brief Determines the next vertex to visit in the Nemertea algorithm.
 *
 * @param prev Pointer to the previous vertex visited (can be nullptr if none).
 * @param current Pointer to the current vertex.
 * @return Pointer to the next vertex to visit, or nullptr if no valid next vertex exists.
 */
Vertex *NextVertex(const Vertex *prev, const Vertex *current);

#endif // NEMERTEA_H_