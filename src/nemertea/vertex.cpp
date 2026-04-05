// -*- coding: utf-8 -*-
// ============================================================================
// Project: Nemertea
// File: vertex.cpp
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
// Implementation of the Vertex class. Manages the internal states of
// individual nodes within the NBFS (Nemertea Breadth-First Search).
// ============================================================================

#include "vertex.hpp"

void Vertex::Connect(Vertex *target)
{
    neighbors_.push_back(target);
};

std::string stateToString(const State state)
{
    switch (state)
    {
    case State::NONE:       //
        return "NONE";      // Not connected
        [[fallthrough]];    //
    case State::CONQUERED:  //
        return "CONQUERED"; // Connected and active
        [[fallthrough]];    //
    case State::INACTIVE:   //
        return "INACTIVE";  // Connected but inactive
        [[fallthrough]];    //
    default:                //
        return "UNKNOWN";   // Some error
    }
}
