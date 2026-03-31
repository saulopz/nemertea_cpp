#ifndef NBFS_HPP_
#define NBFS_HPP_

#include "graph.hpp"
#include <cstddef>
#include <cstdint>
#include <queue>

class Proboscis
{
  private:
    Graph *graph_;                // Graph
    Vertex *root_;                // root vertex on the NBFS search
    uint64_t generation_;          // current id of NBFS search
    std::queue<Vertex *> leaves_; // leaves of the NBFS search

    std::pair<Vertex *, bool> Probe(Vertex *vertex, Vertex *target, bool first) const;

    // MakePath
    size_t Retract(Vertex *vertex) const;

  public:
    Proboscis(Graph *graph) : graph_(graph), root_(nullptr) {};

    size_t Evert(Vertex *startVertex, bool first, size_t depth);
};

#endif // NBFS_HPP_