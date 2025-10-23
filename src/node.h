// -*- coding: utf-8 -*-
// node.h
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

#ifndef NODE_H_
#define NODE_H_

#include "vertex.h"

/**
 * Represents a node in the search tree used in the NBFS algorithm.
 *
 * Each node contains a vertex, a pointer to its parent node, the edge connecting
 * it to its parent, and a list of its children nodes.
 * 
 * Attributes
 * ----------
 * - vertex_: Pointer to the vertex associated with this node.
 * - parent_: Pointer to the parent node.
 * - edge_: Pointer to the edge connecting this node to its parent.
 * - children_: Vector of pointers to the child nodes.
 */
class Node
{
private:
    Vertex *vertex_;               // Vertex of Node
    Node *parent_;                 // Node parent
    Edge *edge_;                   // Edge to parent
    std::vector<Node *> children_; // List of children

public:
    /**
     * @brief Node constructor
     *
     * @param vertex Vertex of the node
     */
    explicit Node(Vertex *vertex) : vertex_(vertex), parent_(), edge_(), children_()
    {
    }

    /**
     * @brief Adds a child node to the current node.
     *
     * @param vertex Vertex of the child node
     * @return Pointer to the newly created child node
     */
    Node *AddChild(Vertex *vertex);

    /**
     * @brief Retrieves the vertex associated with the current node.
     *
     * @return Pointer to the current node
     */
    [[nodiscard]] Vertex *GetVertex() const
    {
        return vertex_;
    }

    /**
     * @brief Retrieves the parent node of the current node.
     *
     * @return Pointer to the parent node
     */
    [[nodiscard]] Node *GetParent() const
    {
        return parent_;
    }

    /**
     * @brief Retrieves the edge connecting the current node to its parent.
     *
     * @return Pointer to the connecting edge
     */
    [[nodiscard]] Edge *GetEdge() const
    {
        return edge_;
    }

    /**
     * @brief Destructor for the Node class.
     *
     * Cleans up the resources used by the node.
     */
    ~Node();
};

#endif // NODE_H_
