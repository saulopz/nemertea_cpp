// -*- coding: utf-8 -*-
// ============================================================================
// Project: Nemertea
// File: proboscis.cpp
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
// Implementation of the Proboscis class, which implements the Nemertea BFS
// algorithm for finding the shortest path between two vertices in a graph.
// ============================================================================

#include "proboscis.hpp"
#include "graph.hpp"
#include "vertex.hpp"
#include <cstdlib>

size_t Proboscis::Evert(Vertex *startVertex, bool first, size_t depth)
{
    while (!leaves_.empty()) // Resetting leaves
        leaves_.pop();

    generation_++;                                // New generation
    root_ = startVertex;                          // Start vertex
    root_->AddToGeneration(nullptr, generation_); // Add root_ to this generation
    root_->SetState(State::CONQUERED);            // Starting as conquered
    leaves_.push(root_);                          // And put in leaves queue
    size_t level = 0;                             // Starting level 0 on tree
    while (!leaves_.empty() && level < depth)
    {
        // It retrieves the number of nodes at that level so we know
        // when we are changing levels. Inside next loop new nodes
        // can be insert, but it is for next level.

        size_t nodes_at_this_level = leaves_.size();

        for (size_t i = 0; i < nodes_at_this_level; i++)
        {
            auto vertex = leaves_.front(); // Get the vertex from de head of leaves
            leaves_.pop();                 // Remove from queue

            auto neighbor_count = vertex->GetNeighborsCount();
            for (size_t j = 0; j < neighbor_count; j++) // While has a possible neighbord
            {
                auto target = vertex->GetNeighbor(j);
                auto [child, found] = Probe(vertex, target, first);
                if (found)                 // If found
                    return Retract(child); // Found the goal
                else if (child)            // If not found, but it is a valid vertex to follow
                    leaves_.push(child);   // Put this vertex (node) in the tail
            }
        }
        level++; // Go to the next level of the tree
    }
    return 0;
}

std::pair<Vertex *, bool> Proboscis::Probe(Vertex *vertex, Vertex *target, bool first) const
{
    const auto vertex_id = vertex->GetId();
    const auto target_id = target->GetId();
    const auto root_id = root_->GetId();

    // Case 1: Edge already active, ignore
    if (graph_->GetEdgeState(vertex_id, target_id) == State::CONQUERED)
        return {nullptr, false};

    if (target->GetState() != State::CONQUERED && target->IsTesting(generation_))
    {
        // Case 2: General blocking of TESTING outside of the first iteration.
        if (!first)
            return {nullptr, false};

        // Case 3: Ancestor locking in the tree during only the "first iteration".
        //         Verify if it's ancestor.
        auto local = vertex->GetParent();
        while (local)
        {
            if (local->GetId() == target_id)
            {
                return {nullptr, false};
            }
            local = local->GetParent();
        }
        // Not ancestor. Let's pass to the next case.
    }

    // Case 4: Ignore returning to father
    auto parent = vertex->GetParent();
    if (parent && target_id == parent->GetId())
        return {nullptr, false};

    // Case 5: If target is root and its first iteraction then found first cycle
    if (first && target_id == root_id)
    {
        target->AddToGeneration(vertex, generation_);
        return {target, true};
    }

    // Case 6 and 7: If vertex is active and is neighbor of root, path found or ignored
    if (target->GetState() == State::CONQUERED)
    {
        // Case 6: vertex is ACTIVE and it's a root neighbor, than i found target
        if (graph_->GetEdgeState(target_id, root_id) == State::CONQUERED)
        {
            target->AddToGeneration(vertex, generation_);
            return {target, true};
        }
        // Case 7: The adjacent vertex is ACTIVE. It's alread in frontier and it's not valid.
        return {nullptr, false};
    }

    // Case 8: Free vertex, add as child
    target->AddToGeneration(vertex, generation_); // Set target as TESTING
    return {target, false};
}

size_t Proboscis::Retract(Vertex *vertex) const
{
    // If there is an active connection to the root,
    // disconnect it to open access to the new region.
    const auto vertex_id = vertex->GetId();
    const auto root_id = root_->GetId();
    if (vertex->GetState() == State::CONQUERED && vertex_id != root_id)
        if (graph_->GetEdgeState(root_id, vertex_id) == State::CONQUERED)
            graph_->SetEdgeState(root_id, vertex_id, State::INACTIVE);

    // Returns to the root and activates the vertices and edges along the path.
    size_t new_vertex_count = 0;
    auto local = vertex;
    do
    {
        if (local->GetState() != State::CONQUERED)
        {
            local->SetState(State::CONQUERED);
            new_vertex_count++;
        }

        // Change connection to parent as ACTIVE
        if (auto parent = local->GetParent())
        {
            graph_->SetEdgeState(local->GetId(), parent->GetId(), State::CONQUERED);
            local = parent; // Go to parent
        }
        else
        {
            break;
        }

    } while (local->GetId() != root_id);

    return new_vertex_count;
}