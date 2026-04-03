#ifndef NEMERTEA_HPP_
#define NEMERTEA_HPP_

#include "graph.hpp"
#include <cstddef>

/**
 * Nemertea is a class that implements the Nemertea algorithm for finding the
 * hamiltonian cycle in a graph. The algorithm is based on the concept of a
 * proboscis, which is a structure that extends from a vertex and explores
 * the graph in a breadth-first manner. The Nemertea class uses the Spot
 * method to identify the next vertex to explore based on the current vertex
 * and the previous vertex. The FirstPath method is used to find the first
 * path from the root vertex to a target vertex, and the Walk method is used
 * to perform the Nemertea walk through the graph, returning the length of the
 * path found.
 * 
 * Attributes
 * ----------
 * - graph_ : The graph on which the algorithm operates.
 */
class Nemertea
{
  private:
    Graph *graph_;

    /**
     * Spots the next vertex to explore based on the current vertex and the
     * previous vertex. This method is used to determine the next vertex to
     * explore during the Nemertea walk through the graph. It takes into
     * account the current vertex, the previous vertex, and the structure of
     * the graph to identify the next vertex to explore.
     * @param prev The previous vertex in the path.
     * @param current The current vertex in the path.
     * @return The next vertex to explore.  
     */
    Vertex *Spot(const Vertex *prev, const Vertex *current) const;

    /**
     * Finds the first path from the root vertex to a target vertex. This
     * method is used to determine the length of the path from the root vertex
     * to a target vertex during the Nemertea walk through the graph. It
     * explores the graph starting from the root vertex and returns the length
     * of the first path found to a target vertex.
     * @param root The root vertex from which to start the search.
     * @return The length of the first path found from the root vertex to a target vertex.
     */
    size_t FirstPath(Vertex *root) const;

  public:

    /**
     * Constructs a Nemertea object with the given graph.
     * @param graph The graph on which the algorithm will operate.
     */
    Nemertea(Graph *graph) : graph_(graph) {};

    /**
     * Performs the Nemertea walk through the graph, returning the length of
     * the path found. This method is used to execute the Nemertea algorithm
     * and find the length of the path from the root vertex to a target vertex.
     * It takes into account the depth of the walk and whether to allow cycles
     * in the path. The method returns the length of the path found during the
     * Nemertea walk through the graph.
     * @param depth The maximum depth to explore during the walk.
     * @param cycle A boolean indicating whether to allow cycles in the path.
     * @return The length of the path found during the Nemertea walk through the graph.
     */
    size_t Walk(size_t depth, bool cycle) const;
};

#endif // NEMERTEA_HPP_