// -*- coding: utf-8 -*-
// nemertea.cpp
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

#include "nemertea.h"
#include "nbfs.h"

size_t FirstPath(Vertex *root);

size_t Nemertea(Graph *graph, const size_t depth, const bool cycle)
{
    Vertex *prev = nullptr;                             // Previous vertex, starts null
    const auto vertex_count = graph->GetVertexCount();  // Number of vertices in the graph
    auto first_vertex = graph->GetRandomVertex();       // Take a random vertex
    const auto first_vertex_id = first_vertex->GetId(); // Get its id
    first_vertex->SetState(State::ACTIVE);              // The first vertex is ACTIVE
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
            auto nbfs = NBFS(current, first, depth);
            size = nbfs.Run();
            path_count += size;
            first = false;
        } while (size > 0);

        const auto next = NextVertex(prev, current);
        prev = current;
        current = next;
    } while (current && current->GetId() != first_vertex_id && path_count < vertex_count);

    // If the current id is equal to the first vertex id, return to the starting vertex - finish
    // if pathCount is equal to vertexCount, all vertices are active - finish

    return path_count;
}

Vertex *NextVertex(const Vertex *prev, const Vertex *current)
{
    const auto prev_id = prev ? prev->GetId() : 0;
    const auto edge_count = current->GetEdgeCount();
    for (size_t i = 0; i < edge_count; i++)
    {
        auto e = current->GetEdge(i);
        if (e && e->GetState() == State::ACTIVE)
        {
            auto adjacent = current->GetAdjacent(e);
            if (!prev || adjacent->GetId() != prev_id)
                return adjacent;
        }
    }
    return nullptr;
}

size_t FirstPath(Vertex *root)
{
    size_t count = 0;
    auto p = root;
    bool found = false;
    do
    {
        found = false;
        size_t edge_count = p->GetEdgeCount();
        size_t i = 0;
        while (i < edge_count && !found)
        {
            auto edge = p->GetEdge(i);
            if (edge->GetState() == State::NONE)
            {
                auto adjacent = p->GetAdjacent(edge);
                if (adjacent->GetState() == State::NONE)
                {
                    edge->SetState(State::ACTIVE);
                    adjacent->SetState(State::ACTIVE);
                    count++;
                    p = adjacent;
                    found = true;
                }
            }
            i++;
        }
    } while (found);
    return count;
}