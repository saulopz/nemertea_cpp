#ifndef GRAPH_H_
#define GRAPH_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include "vertex.h"
#include "edge.h"

class Graph
{
private:
    std::string filename_;           // File name of graph
    std::vector<Vertex *> vertices_; // List of vertices
    std::vector<Edge *> edges_;      // List of Edges

public:
    Graph(std::string filename)
        : filename_(std::move(filename)),
          vertices_(),
          edges_() {}
    void Load();
    size_t GetVertexCount() { return vertices_.size(); };
    Vertex *GetRandomVertex()
    {
        return vertices_.empty() ? nullptr : vertices_[rand() % vertices_.size()];
    }
    Vertex *GetVertexById(uint64_t id);
    std::vector<Vertex *> &GetVertices() { return vertices_; };
    void Save(bool solved);
    ~Graph();
};

#endif // GRAPH_H_