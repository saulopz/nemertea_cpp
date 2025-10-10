#ifndef NEMERTEA_H_
#define NEMERTEA_H_

#include "graph.h"

size_t Nemertea(Graph* graph, size_t depth, bool cycle);

Vertex* NextVertex(const Vertex* prev, const Vertex* current);

#endif // NEMERTEA_H_