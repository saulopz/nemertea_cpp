// -*- coding: utf-8 -*-
// vertex.cpp
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

#include "vertex.h"

Vertex *Vertex::GetAdjacent(const Edge *edge) const
{
    const auto a = edge->GetA();
    const auto b = edge->GetB();
    if (a->id_ == id_)
        return b;
    if (b->id_ == id_)
        return a;
    return nullptr;
}

void Vertex::AddEdge(Edge *edge)
{
    edges_.push_back(edge);
};

Edge *Vertex::GetEdgeTo(const Vertex *v) const
{
    if (!v)
        return nullptr;
    for (const auto &edge : edges_)
        if (edge->Contains(v))
            return edge;
    return nullptr;
}

bool Vertex::IsConnected(const Vertex *v) const
{
    return GetEdgeTo(v) ? true : false;
}

void Vertex::ChangeActiveEdge(const State current_state, State new_state)
{
    if (current_state != State::ACTIVE && new_state == State::ACTIVE)
    {
        active_edge_count_++;
        return;
    }
    if (current_state == State::ACTIVE && new_state != State::ACTIVE)
        active_edge_count_--;
}

Vertex::~Vertex()
{
    edges_.clear();
}
