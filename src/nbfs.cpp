// -*- coding: utf-8 -*-
// nbfs.cpp
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

#include "nbfs.h"
#include "node.h"

NBFS::NBFS(Vertex *startVertex, const bool first, const size_t depth) : leaves_(), first_(first), depth_(depth)
{
    root_ = new Node(startVertex);
    leaves_.push_back(root_);
}

size_t NBFS::Run()
{
    size_t level = 0;
    std::vector<Node *> new_leaves;
    while (!leaves_.empty() && level < depth_)
    {
        for (const auto &leaf : leaves_)
        {
            Vertex *vertex = leaf->GetVertex();
            const size_t edge_count = vertex->GetEdgeCount();
            for (size_t i = 0; i < edge_count; i++)
            {
                Edge *edge = vertex->GetEdge(i);
                auto [child, found] = SelectChild(leaf, edge, first_);
                if (found)
                    return MakePath(child);
                else if (child)
                    new_leaves.push_back(child);
            }
        }
        leaves_ = std::move(new_leaves);
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

    // Case 2: Only in the first iteration can you visit vertices with TESTING state
    if (!first && adjacent->GetState() == State::TESTING)
        return {nullptr, false};

    // Case 3: Ignore returning to father
    Node *parent = node->GetParent();
    if (parent && adjacent->GetId() == parent->GetVertex()->GetId())
        return {nullptr, false};

    // Case 4: If it is root with no active connections yet
    Vertex *root_vertex = root_->GetVertex();
    if (adjacent->GetId() == root_vertex->GetId() && root_vertex->GetActiveEdgeCount() == 0)
        return {node->AddChild(adjacent), true};

    // Case 5 and 6: If vertex is active and is neighbor of root, path found or ignored
    if (adjacent->GetState() == State::ACTIVE)
    {
        auto edge_to_root = adjacent->GetEdgeTo(root_->GetVertex());
        if (edge_to_root && edge_to_root->GetState() == State::ACTIVE)
            return {node->AddChild(adjacent), true};
        return {nullptr, false};
    }

    // Case 7: Free vertex, add as child
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
    leaves_.clear();
}
