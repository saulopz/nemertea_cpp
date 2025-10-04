#ifndef NEMERTEA_H_
#define NEMERTEA_H_

#include "vertex.h"
#include "graph.h"

int Nemertea(Graph* graph, int depth);
Vertex* NextVertex(Vertex* prev, Vertex* current);

#endif // NEMERTEA_H_