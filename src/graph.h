#ifndef GRAPH_H_
#define GRAPH_H_

#include <memory>
#include <vector>
#include <random>
#include "vertex.h"
#include "edge.h"

class Graph
{
private:
    std::string filename_; // File name of graph
    std::vector<Vertex *> vertices_; // List of vertices
    std::vector<Edge *> edges_; // List of Edges
    std::mt19937 random_engine_{std::random_device{}()};
    std::uniform_int_distribution<size_t> index_distribution_;

public:
    explicit Graph(std::string filename)
        : filename_(std::move(filename)),
          vertices_(),
          edges_()
    {
    }

    void Load();

    [[nodiscard]] size_t GetVertexCount() const { return vertices_.size(); };

    [[nodiscard]] Vertex *GetRandomVertex()
    {
        if (vertices_.empty())
            return nullptr;

        const size_t random_index = index_distribution_(random_engine_);
        return vertices_[random_index];
    }

    [[nodiscard]] Vertex *GetVertexById(uint64_t id) const;

    std::vector<Vertex *> &GetVertices() { return vertices_; };

    void Save(bool solved);

    ~Graph();
};

#endif // GRAPH_H_
