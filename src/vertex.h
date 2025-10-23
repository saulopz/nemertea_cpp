// -*- coding: utf-8 -*-
// vertex.h
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

#ifndef VERTEX_H_
#define VERTEX_H_

#include "edge.h"
#include <string>
#include <utility>
#include <vector>

/**
 * Represents a vertex 'v' of a graph G(V, E) | v ∈ V.
 *
 * Stores incident edges and logical state used by the Nemertea
 * and NBFS algorithms.
 *
 * This class is part of the Nemertea project.
 * Created by Saulo Popov Zambiasi, 2025.
 *
 * Attributes
 * ----------
 * - id_: identifier of a unique vertex.
 * - name_: vertex name or description.
 * - x_, y_: spatial position of vertex.
 * - active_edge_count_: counter of active edges
 * - edges_: list of edges connected
 * - state_: state of vertex (NONE | TESTING | ACTIVE)
 */
class Vertex
{
private:
    /** @brief identifier of a unique vertex. */
    uint64_t id_;               // Vertex id
    std::string name_;          // Vertex name or description
    double x_, y_;              // Vertex position on 2D space
    size_t active_edge_count_;  // Number of active edges
    std::vector<Edge *> edges_; // List of edges connected
    State state_;               // Vertex state (NONE, ACTIVE, TESTING)

public:
    /**
     * Vertex constructor
     *
     * @param id unique identifer.
     * @param name vertex name.
     * @param x horizontal position of x.
     * @param y vertical position of x.
     */
    Vertex(const uint64_t id, std::string name, const double x, const double y)
        : id_(id), name_(std::move(name)), x_(x), y_(y), active_edge_count_(0), edges_(), state_(State::NONE)
    {
    }

    /**
     * Get the vertex identifier.
     *
     * @return The unique identifier.
     */
    [[nodiscard]] uint64_t GetId() const
    {
        return id_;
    };

    /**
     * Get vertex name or description.
     *
     * @return Pointer to string of vertex name.
     */
    [[nodiscard]] const std::string &GetName() const
    {
        return name_;
    };

    /**
     * Get vertex horizontal position of vertex.
     *
     * @return The horizontal position.
     */
    [[nodiscard]] double GetX() const
    {
        return x_;
    };

    /**
     * Get vertex vertical position of vertex.
     *
     * @return The vertical position.
     */
    [[nodiscard]] double GetY() const
    {
        return y_;
    };

    /**
     * Get vertex state (NONE, TESTING, ACTIVE)
     *
     * @return State
     */
    [[nodiscard]] State GetState() const
    {
        return state_;
    };

    /**
     * Get count of children edges of vertex
     *
     * @return count
     */
    [[nodiscard]] size_t GetEdgeCount() const
    {
        return edges_.size();
    };

    /**
     * Get edge child in position index
     *
     * @param index position of edge in children list
     *
     * @return edge
     */
    [[nodiscard]] Edge *GetEdge(const size_t index) const
    {
        return index < edges_.size() ? edges_[index] : nullptr;
    };

    /**
     * @brief
     *
     * @param edge the edge to get other vertex.
     * @return The vertex on the other side of the edge,
     * if any. Otherwise, returns null.
     */
    [[nodiscard]] Vertex *GetAdjacent(const Edge *edge) const;

    /**
     * @brief Returns the edge that connects this vertex with other.
     *
     * @param v other vertex.
     * @return The edge of connection if exists, otherwise nullptr.
     */
    [[nodiscard]] Edge *GetEdgeTo(const Vertex *v) const;

    /**
     * @brief Verifies if this vertex is connect whit other
     *
     * @param v other vertex.
     * @return If is connect or not.
     */
    [[nodiscard]] bool IsConnected(const Vertex *v) const;

    /**
     * @brief Returns the amount of edges children with state ACTIVE
     *
     * @return active edge count
     */
    [[nodiscard]] size_t GetActiveEdgeCount() const
    {
        return active_edge_count_;
    };

    /**
     * @brief Change the active edge count.
     *
     * Changes the active edge count. If the edge's state was not active and
     * is now active, then it increments the active edge count. If it was
     * active and the new state is not active, then it decrements the counter.
     *
     * @param current_state current state of edge.
     * @param new_state new state of edge.
     */
    void ChangeActiveEdge(State current_state, State new_state);

    /**
     * @brief Change vertex state
     *
     * @param state new vertex state
     */
    void SetState(const State state)
    {
        state_ = state;
    };

    /**
     * @brief Adds an edge to the vertex edge list.
     *
     * @param edge the edge to add.
     */
    void AddEdge(Edge *edge);

    /**
     * @brief The vertex destructor clears the edge list.
     */
    ~Vertex();
};

#endif // VERTEX_H_
