#ifndef NBFS_HPP_
#define NBFS_HPP_

#include "graph.hpp"
#include <cstddef>
#include <cstdint>
#include <queue>

class NBFS
{
  private:
    Graph *graph_;                // Graph
    Vertex *root_;                // root vertex on the NBFS search
    uint64_t search_id_;          // current id of NBFS search
    std::queue<Vertex *> leaves_; // leaves of the NBFS search

    std::pair<Vertex *, bool> SelectChild(Vertex *vertex, Vertex *target, bool first) const;

    size_t MakePath(Vertex *vertex) const;

  public:
    NBFS(Graph *graph) : graph_(graph), root_(nullptr) {};

    size_t Run(Vertex *startVertex, bool first, size_t depth);
};

#endif // NBFS_HPP_