#ifndef NBFS_HPP_
#define NBFS_HPP_

#include "graph.hpp"
#include <cstddef>
#include <cstdint>
#include <queue>

class NBFS
{
  private:
    Graph *graph_;
    Vertex *root_;
    uint64_t search_id_;
    std::queue<Vertex *> leaves_;

    std::pair<Vertex *, bool> SelectChild(Vertex *vertex, Vertex *target, bool first) const;

    size_t MakePath(Vertex *vertex) const;

  public:
    NBFS(Graph *graph) : graph_(graph), root_(nullptr) {};

    size_t Run(uint64_t startVertex, bool first, size_t depth);
};

#endif // NBFS_HPP_