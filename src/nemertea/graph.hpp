#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include "vertex.hpp"
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>
#include <iostream>

class Graph
{
  private:
    std::string filename_;           // File name of graph
    std::vector<Vertex *> vertices_; // List of vertices
    std::mt19937 random_engine_{std::random_device{}()};
    std::uniform_int_distribution<size_t> index_distribution_;
    std::vector<State> adj_;
    size_t vector_size_;

    inline void SetAdjacent(size_t a, size_t b)
    {
        // Avoid repeated access readings
        size_t n = vector_size_;
        auto data = adj_.data();
        data[a * n + b] = State::INACTIVE;
        data[b * n + a] = State::INACTIVE;
    }

  public:
    explicit Graph(std::string filename) : filename_(std::move(filename)), vertices_()
    {
    }
    void Load();
    void Save(bool solved);
    ~Graph();

    [[nodiscard]] Vertex *GetRandomVertex();

    [[nodiscard]] inline Vertex *GetVertex(uint64_t id) const
    {
        if (id >= vertices_.size())
        {
            std::cerr << "ERRO CRÍTICO: Tentativa de acessar vértice " << id << " em um grafo de tamanho "
                      << vertices_.size() << std::endl;
            exit(1);
        }
        return vertices_[id];
    }

    [[nodiscard]] inline bool IsAdjacent(uint64_t a, uint64_t b) const
    {
        return adj_[a * vector_size_ + b] != State::NONE;
    }

    void SetConnectionState(uint64_t a, uint64_t b, State state)
    {
        size_t n = vector_size_;
        auto data = adj_.data();
        data[a * n + b] = state;
        data[b * n + a] = state;
    }

    [[nodiscard]] inline State GetConnectionState(uint64_t a, uint64_t b)
    {
        return adj_[a * vector_size_ + b];
    }

    [[nodiscard]] inline size_t GetVertexCount() const
    {
        return vector_size_;
    };

    [[nodiscard]] inline std::vector<Vertex *> &GetVertices()
    {
        return vertices_;
    };
};

#endif // GRAPH_HPP_
