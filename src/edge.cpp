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

#include "edge.h"
#include "vertex.h"

void Edge::SetState(const State state)
{
    if (!a_ || !b_)
        return;
    // First change active edge of its connected vertices
    a_->ChangeActiveEdge(state_, state);
    b_->ChangeActiveEdge(state_, state);
    // Then change state
    state_ = state;
};
