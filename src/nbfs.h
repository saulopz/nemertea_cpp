// -*- coding: utf-8 -*-
// nbfs.h
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

#ifndef NBFS_H_
#define NBFS_H_

#include "node.h"
#include "vertex.h"
#include <utility>

/**
 * Class that implements the Nemertea BFS (NBFS) algorithm.
 *
 * The NBFS algorithm is a specialized breadth-first search
 * designed to explore graph structures for the Hamiltonian Cycle Problem.
 *
 * Attributes
 * ----------
 * - root_: The origin node from which the search begins.
 * - leaves_: A list of leaf nodes reached during the search.
 * - first_: A boolean flag indicating if it's the first interaction.
 * - depth_: The maximum depth limit for the search.
 */
class NBFS
{
  private:
    Node *root_;   // Origin node (NBFS is owner)
    bool first_;   // If is first interaction
    size_t depth_; // Limit of depth

    // Private methods

    /**
     * @brief Selects a child node based on the given parameters.
     *
     * @param node The current node.
     * @param edge The edge to consider for selection.
     * @param first A boolean indicating if it's the first interaction.
     * @return A pair containing the selected child node and a boolean
     * indicating success.
     */
    std::pair<Node *, bool> SelectChild(Node *node, Edge *edge, bool first) const;

    /**
     * @brief Constructs the path from the given node to the root.
     *
     * @param node The node from which to start constructing the path.
     * @return The size of the constructed path.
     */
    size_t MakePath(const Node *node) const;

  public:
    /**
     * @brief Constructor for the NBFS class.
     *
     * @param startVertex The starting vertex for the search.
     * @param first A boolean indicating if it's the first interaction.
     * @param depth The maximum depth limit for the search.
     */
    NBFS(Vertex *startVertex, bool first, size_t depth) //
        : root_(new Node(startVertex)),                 //
          first_(first),                                //
          depth_(depth) {};

    /**
     * @brief Executes the NBFS algorithm.
     *
     * @return The size of the path found.
     */
    size_t Run();

    /**
     * @brief Destructor for the NBFS class.
     */
    ~NBFS();
};

#endif // NBFS_H_
