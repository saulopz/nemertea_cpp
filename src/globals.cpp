// -*- coding: utf-8 -*-
// globals.cpp
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

#include "globals.h"

std::string stateToString(const State state)
{
    switch (state)
    {
    case State::ACTIVE:
        return "ACTIVE";
    case State::NONE:
        return "INACTIVE";
    case State::TESTING:
        return "TESTING";
    default:
        return "UNKNOWN";
    }
};
