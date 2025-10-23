// -*- coding: utf-8 -*-
// globals.h
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

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <string>

enum class State
{
    NONE = 0,
    TESTING = 1,
    ACTIVE = 2
};

std::string stateToString(State state);

#endif // GLOBALS_H_
