// -*- coding: utf-8 -*-
// node.cpp
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

#include "node.h"
#include "globals.h"

Node *Node::AddChild(Vertex *vertex)
{
    if (!vertex)
        return nullptr;
    const auto child = new Node(vertex);
    child->parent_ = this;
    child->edge_ = vertex_->GetEdgeTo(vertex);
    children_.push_back(child);
    return child;
}

Node::~Node()
{
    if (edge_ && edge_->GetState() != State::ACTIVE)
        edge_->SetState(State::NONE);
    if (vertex_ && vertex_->GetState() != State::ACTIVE)
        vertex_->SetState(State::NONE);
    for (const auto child : children_)
        delete child;
    children_.clear();
}
