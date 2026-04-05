// -*- coding: utf-8 -*-
// ============================================================================
// Project: Nemertea
// File: graph.hpp
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
// Declaration of the Graph class, which represents a graph structure consisting
// of vertices and edges, and provides methods for loading, saving, and managing
// the graph. The Graph class uses an adjacency matrix to track the state of
// connections between vertices, allowing for efficient updates and queries of
// connection states. The class also includes functionality for random vertex
// selection, which can be useful for various graph algorithms and operations.
// ============================================================================

#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include "vertex.hpp"
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>
#include <iostream>

/**
 * Represents a graph structure consisting of vertices and edges (G=(V,E)).
 * The graph can be loaded from a file, and it provides methods for accessing
 * vertices, managing connections, and saving the graph state. The graph uses
 * an adjacency matrix to track the state of connections between vertices,
 * allowing for efficient updates and queries of connection states. The graph
 * also includes functionality for random vertex selection, which can be
 * useful for various graph algorithms and operations.
 *
 * Attributes
 * ----------
 * - filename_ : The name of the file from which the graph is loaded.
 * - vertices_ : A list of pointers to the vertices in the graph.
 * - adj_      : An adjacency matrix represented as a vector of States, where
 *               each entry indicates the state of the connection between two
 *               vertices.
 * - vector_size_: The number of vertices in the graph, used to calculate
 *                 indices in the adjacency matrix.
 */
class Graph
{
private:
    std::string filename_;           // File name of graph
    std::vector<Vertex *> vertices_; // List of vertices
    std::vector<State> adj_;         // Adjacency matrix to track connection states
    size_t vector_size_;             // Number of vertices, used for adjacency matrix indexing

    // Random number generator
    std::mt19937 random_engine_{std::random_device{}()};
    std::uniform_int_distribution<size_t> index_distribution_;

public:
    /**
     * Constructs a new Graph object with the specified filename. The graph will
     * be loaded from the file when the Load() method is called. The constructor
     * initializes the filename and prepares the graph for loading, but does not
     * perform any loading operations itself.
     *
     * @param filename The name of the file from which to load the graph.
     */
    explicit Graph(std::string filename) : filename_(std::move(filename)), vertices_()
    {
    }

    /**
     * Loads the graph from the specified a json file. This method reads the graph
     * data from the file, initializes the vertices and the adjacency matrix based
     * on the graph structure defined in the file.
     */
    void Load();

    /**
     * Saves the graph to dot format. This method outputs the graph structure in a
     * format that can be visualized using graph visualization tools. The 'solved'
     * parameter indicates whether the graph has been solved, which may affect how
     * the graph is represented in the output (e.g., highlighting certain vertices
     * or edges).
     */
    void Save(bool solved);

    /**
     * Destructs the Graph object, releasing any resources associated with the graph,
     * such as dynamically allocated memory for vertices and the adjacency matrix.
     */
    ~Graph();

    /**
     * Returns a random vertex from the graph. This method uses a random number
     * generator to select a vertex from the list of vertices in the graph. The random
     * selection can be useful for various graph algorithms that require random sampling
     * of vertices.
     *
     * @return A pointer to a randomly selected vertex from the graph.
     */
    [[nodiscard]] Vertex *GetRandomVertex();

    /**
     * Sets the state of the edge between two vertices (a and b) in the adjacency matrix.
     * This method updates the state of the connection between the specified vertices
     * in the adjacency matrix, allowing for efficient tracking of connection states. The
     * state parameter indicates the new state of the edge, which can be used to represent
     * different types of connections (e.g., active, conquered, inactive) between the
     * vertices. For now, we consider the graph to be undirected, therefore we update both
     * entries in the adjacency matrix to maintain consistency.
     *
     * @param a The ID of the first vertex.
     * @param b The ID of the second vertex.
     * @param state The new state to set for the edge between vertices a and b.
     */
    void SetEdgeState(uint64_t a, uint64_t b, State state)
    {
        size_t n = vector_size_;
        auto data = adj_.data();
        data[a * n + b] = state;
        data[b * n + a] = state;
    }

    /**
     * Returns the state of the edge between two vertices (a and b) from the adjacency matrix.
     * This method retrieves the current state of the connection between the specified vertices
     * from the adjacency matrix, allowing for efficient queries of connection states. The
     * state returned indicates the current status of the edge between the vertices, which can
     * be used to determine how the vertices are connected (e.g., active, conquered, inactive)
     * in the graph. For an undirected graph, the state of the edge should be the same regardless
     * of the order of the vertices (a and b), so we can simply return the state from one of the
     * entries in the adjacency matrix.
     *
     * @param a The ID of the first vertex.
     * @param b The ID of the second vertex.
     * @return The current state of the edge between vertices a and b.
     */
    [[nodiscard]] inline State GetEdgeState(uint64_t a, uint64_t b)
    {
        return adj_[a * vector_size_ + b];
    }

    /**
     * Returns the number of vertices in the graph. This method provides a way to retrieve the
     * total count of vertices in the graph, which can be useful for various graph algorithms
     * and operations that require knowledge of the graph's size. The vertex count is stored
     * in the vector_size_ attribute, which is initialized when the graph is loaded and can be
     * used to manage the adjacency matrix and other graph-related data structures efficiently.
     *
     * @return The number of vertices in the graph.
     */
    [[nodiscard]] inline size_t GetVertexCount() const
    {
        return vector_size_;
    };
};

#endif // GRAPH_HPP_
