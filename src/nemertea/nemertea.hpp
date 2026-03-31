#ifndef NEMERTEA_HPP_
#define NEMERTEA_HPP_

#include "graph.hpp"
#include <cstddef>

class Nemertea
{
  private:
    Graph *graph_;

    // Identifica para onde deve seguir (s)
    Vertex *Spot(const Vertex *prev, const Vertex *current) const;

    size_t FirstPath(Vertex *root) const;

  public:
    Nemertea(Graph *graph) : graph_(graph) {};

    size_t Walk(size_t depth, bool cycle) const;
};

#endif // NEMERTEA_HPP_