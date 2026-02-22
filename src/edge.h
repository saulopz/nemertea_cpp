// -*- coding: utf-8 -*-
// edge.h
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

#ifndef EDGE_H_
#define EDGE_H_

#include "globals.h"
#include <cstdint>
#include <memory>

class Vertex;

/**
 * Represents an edge 'e' of a graph G(V, E) | e ∈ E.
 *
 * Connects two vertices and stores weight and logical state
 * used by the Nemertea and NBFS algorithms.
 *
 * This class is part of the Nemertea project.
 * Created by Saulo Popov Zambiasi, 2025.
 *
 * Attributes
 * ----------
 * - id_: identifier of a unique edge.
 * - a_, b_: vertices connected by this edge.
 * - weight_: weight of connection.
 * - state_: state of edge (NONE | TESTING | ACTIVE)
 */
class Edge
{
  private:
    uint64_t id_;    // Edge id
    Vertex *a_, *b_; // This edge connects with 2 vertices
    int weight_;     // weight of connection
    State state_;    // Edge state (ACTIVE, NONE, TESTING)

  public:
    /**
     * Edge constructor
     * 
     * @param id unique identifier.
     * @param a vertex a.
     * @param b vertex b.
     * @param weight weight of connection.
     */
    Edge(const uint64_t id, Vertex *a, Vertex *b, const int weight)
        : id_(id), a_(a), b_(b), weight_(weight), state_(State::NONE) {};

    /**
     * Get the edge identifier.
     * 
     * @return The unique identifier.
     */
    [[nodiscard]] uint64_t GetId() const
    {
        return id_;
    };

    /**
     * Get edge weight.
     * 
     * @return The weight.
     */
    [[nodiscard]] int GetWeight() const
    {
        return weight_;
    };

    /**
     * Change edge weight.
     * 
     * @param state new weight.
     */
    void SetWeight(const int weight)
    {
        weight_ = weight;
    };

    /**
     * Returns the edge state
     * 
     * @return The edge state
     */
    [[nodiscard]] State GetState() const
    {
        return state_;
    };

    /**
     * Change edge state
     * 
     * @param state new edge state
     */
    void SetState(State state);

    /**
     * Get vertex a
     * 
     * @return vertex a
     */
    [[nodiscard]] Vertex *GetA() const
    {
        return a_;
    };

    /**
     * Get vertex b
     * 
     * @return vertex b
     */
    [[nodiscard]] Vertex *GetB() const
    {
        return b_;
    };

    /**
     * @brief Verifies if the edge contains the vertex v
     * 
     * @param v vertex to check
     * @return true if contains, false otherwise
     */
    [[nodiscard]]
    bool Contains(const Vertex *v) const
    {
        return v ? a_ == v || b_ == v : false;
    }
};

#endif // EDGE_H_
