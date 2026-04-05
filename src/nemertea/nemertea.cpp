// -*- coding: utf-8 -*-
// ============================================================================
// Project: Nemertea
// File: nemertea.cpp
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
// Implementation of the Nemertea class, which implements the Nemertea algorithm
// for finding the hamiltonian cycle in a graph.
// ============================================================================

#include "nemertea.hpp"
#include "proboscis.hpp"

// size_t FirstPath(Graph *graph, Vertex *root);

size_t Nemertea::Walk(const size_t depth, const bool cycle) const
{
    auto proboscis = Proboscis(graph_);
    Vertex *prev = nullptr;                             // Previous vertex, starts null
    const auto vertex_count = graph_->GetVertexCount(); // Number of vertices in the graph
    auto first_vertex = graph_->GetRandomVertex();      // Take a random vertex
    const auto first_vertex_id = first_vertex->GetId(); // Get its id
    first_vertex->SetState(State::CONQUERED);              // The first vertex is ACTIVE
    auto current = first_vertex;                        // Navigation vertex
    size_t path_count = 1;                              // The first vertex is already on the path
    bool first = true;

    // If is a path resolution. Create a first line and then start Nemertea in the root point
    if (!cycle)
    {
        path_count += FirstPath(first_vertex);
        first = false;
    }

    do
    {
        size_t size = 0;
        do
        {
            size = proboscis.Evert(current, first, depth);
            path_count += size;
            first = false;

        } while (size > 0);

        const auto next = Spot(prev, current);
        prev = current;
        current = next;

    } while (current && current->GetId() != first_vertex_id && path_count < vertex_count);

    // If the current id is equal to the first vertex id, return to the starting vertex - finish
    // if pathCount is equal to vertexCount, all vertices are active - finish

    return path_count;
}

Vertex *Nemertea::Spot(const Vertex *prev, const Vertex *current) const
{
    const auto neighbor_count = current->GetNeighborsCount();
    for (size_t i = 0; i < neighbor_count; i++)
    {
        auto neighbor = current->GetNeighbor(i);
        if (graph_->GetEdgeState(current->GetId(), neighbor->GetId()) == State::CONQUERED)
        {
            if (!prev || neighbor->GetId() != prev->GetId())
                return neighbor;
        }
    }
    return nullptr;
}

size_t Nemertea::FirstPath(Vertex *root) const
{
    size_t count = 0;
    auto vertex = root;
    bool found = false;
    do
    {
        found = false;
        size_t neighbor_count = vertex->GetNeighborsCount();
        size_t i = 0;
        while (i < neighbor_count && !found)
        {
            auto neighbor = vertex->GetNeighbor(i);
            if (neighbor->GetState() != State::CONQUERED)
            {
                graph_->SetEdgeState(vertex->GetId(), neighbor->GetId(), State::CONQUERED);
                neighbor->SetState(State::CONQUERED);
                count++;
                vertex = neighbor;
                found = true;
            }
            i++;
        }
    } while (found);
    return count;
}
