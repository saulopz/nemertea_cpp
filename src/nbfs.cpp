// -*- coding: utf-8 -*-
// nbfs.cpp
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

#include "nbfs.h"
#include "node.h"
#include <iostream>
#include <queue>

size_t NBFS::Run()
{
    std::queue<Node *> leaves;

    leaves.push(root_);
    size_t level = 0;
    while (!leaves.empty() && level < depth_)
    {
        // It retrieves the number of nodes at that level
        // so we know when we are changing levels.
        size_t nodes_at_this_level = leaves.size();

        for (size_t i = 0; i < nodes_at_this_level; i++)
        {
            Node *node = leaves.front();
            leaves.pop();

            Vertex *vertex = node->GetVertex();
            const size_t edge_count = vertex->GetEdgeCount();

            for (size_t i = 0; i < edge_count; i++)
            {
                Edge *edge = vertex->GetEdge(i);
                auto [child, found] = SelectChild(node, edge, first_);
                if (found)
                    return MakePath(child);
                else if (child)
                    leaves.push(child);
            }
        }
        level++;
    }
    return 0;
}

std::pair<Node *, bool> NBFS::SelectChild(Node *node, Edge *edge, const bool first) const
{
    // Case 1: Edge already active - ignore
    if (edge->GetState() == State::ACTIVE)
        return {nullptr, false};

    auto adjacent = node->GetVertex()->GetAdjacent(edge);

    if (adjacent->GetState() == State::TESTING)
    {
        // Case 2: General blocking of TESTING outside of the first iteration.
        if (!first)
            return {nullptr, false};

        // Case 3: Ancestor locking in the tree during the first iteration.
        //         Verify if it's ancestor
        const Node *ancestor = node->GetParent();
        while (ancestor)
        {
            if (ancestor->GetVertex()->GetId() == adjacent->GetId())
                return {nullptr, false};
            ancestor = ancestor->GetParent();
            std::cout << "." << std::endl;
        }
        // Not ancestor, let pass to next cases
    }

    // Case 4: Ignore returning to father
    Node *parent = node->GetParent();
    if (parent && adjacent->GetId() == parent->GetVertex()->GetId())
        return {nullptr, false};

    // Case 5: If it is root with no active connections yet
    Vertex *root_vertex = root_->GetVertex();
    if (adjacent->GetId() == root_vertex->GetId() && root_vertex->GetActiveEdgeCount() == 0)
        return {node->AddChild(adjacent), true};

    // Case 6 and 7: If vertex is active and is neighbor of root, path found or ignored
    if (adjacent->GetState() == State::ACTIVE)
    {
        // Case 6: vertex is active and it's a root neighbor, than i found target
        auto edge_to_root = adjacent->GetEdgeTo(root_->GetVertex());
        if (edge_to_root && edge_to_root->GetState() == State::ACTIVE)
            return {node->AddChild(adjacent), true};

        // Case 7: The adjacent vertex is ACTIVE. It's alread in frontier and it's not valid.
        return {nullptr, false};
    }

    // Case 8: Free vertex, add as child
    edge->SetState(State::TESTING);
    adjacent->SetState(State::TESTING);
    return {node->AddChild(adjacent), false};
}

size_t NBFS::MakePath(const Node *node) const
{
    // If there is an active connection to the root, disconnect to open a new region.
    if (node->GetVertex()->GetState() == State::ACTIVE)
    {
        auto edge = root_->GetVertex()->GetEdgeTo(node->GetVertex());
        if (edge && edge->GetState() == State::ACTIVE)
            edge->SetState(State::NONE);
    }

    // It makes its way back to the root and activates the vertices and edges
    size_t new_vertex_count_ = 0;
    auto n = node;
    while (n->GetParent())
    {
        if (n->GetVertex()->GetState() != State::ACTIVE)
            new_vertex_count_++;
        n->GetVertex()->SetState(State::ACTIVE);
        if (n->GetEdge())
            n->GetEdge()->SetState(State::ACTIVE);
        n = n->GetParent();
    }
    return new_vertex_count_;
}

NBFS::~NBFS()
{
    delete root_;
}
