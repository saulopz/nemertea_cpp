// -*- coding: utf-8 -*-
// graph.h
//
// Nemertea: A Territorial Expansion-Based Algorithm
// for the Hamiltonian Cycle Problem
//
// © 2021-Present Saulo Popov Zambiasi. All rights reserved.
// Registered at INPI (Brazil).
// Contact: saulopz@gmail.com
//
// This file is part of the Nemertea source code,
// implementing the Vertex class used in the NBFS algorithm.
//

#ifndef GRAPH_H_
#define GRAPH_H_

#include "edge.h"
#include "vertex.h"
#include <memory>
#include <random>
#include <vector>

/**
 * Graph class representing a collection of vertices and edges.
 *
 * Attributes
 * ----------
 * - filename_: The name of the file associated with the graph.
 * - vertices_: The vertices in the graph.
 * - edges_: The edges in the graph.
 */
class Graph
{
private:
    std::string filename_;           // File name of graph
    std::vector<Vertex *> vertices_; // List of vertices
    std::vector<Edge *> edges_;      // List of Edges
    std::mt19937 random_engine_{std::random_device{}()};
    std::uniform_int_distribution<size_t> index_distribution_;

public:
    /**
     * @brief Graph constructor
     *
     * @param filename The name of the file associated with the graph.
     */
    explicit Graph(std::string filename) : filename_(std::move(filename)), vertices_(), edges_()
    {
    }

    /**
     * @brief Loads the graph from a file.
     */
    void Load();

    /**
     * @brief Get the count of vertices in the graph.
     * 
     * @return The number of vertices.
     */
    [[nodiscard]] size_t GetVertexCount() const
    {
        return vertices_.size();
    };

    /**
     * @brief Get the count of edges in the graph.
     * 
     * @return The number of edges.
     */
    [[nodiscard]] size_t GetEdgeCount() const
    {
        return edges_.size();
    };

    /**
     * @brief Get a random vertex from the graph.
     * 
     * @return A pointer to a random vertex, or nullptr if the graph is empty.
     */
    [[nodiscard]] Vertex *GetRandomVertex()
    {
        if (vertices_.empty())
            return nullptr;

        const size_t random_index = index_distribution_(random_engine_);
        return vertices_[random_index];
    }

    /**
     * @brief Get a vertex by its ID.
     * 
     * @param id The ID of the vertex to find.
     * @return A pointer to the vertex with the specified ID, or nullptr if not found.
     */
    [[nodiscard]] Vertex *GetVertexById(uint64_t id) const;

    /**
     * @brief Get a reference to the vector of vertices.
     * 
     * @return A reference to the vector of vertices.
     */

    std::vector<Vertex *> &GetVertices()
    {
        return vertices_;
    };

    /**
     * @brief Save the graph to a file.
     * 
     * @param solved Indicates whether the graph has been solved.
     */
    void Save(bool solved);

    /**
     * @brief Graph destructor
     */
    ~Graph();
};

#endif // GRAPH_H_
