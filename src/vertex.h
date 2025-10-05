#ifndef VERTEX_H_
#define VERTEX_H_

#include "edge.h"
#include <utility>
#include <vector>
#include <string>

class Vertex
{
private:
    uint64_t id_; // Vertex id
    std::string name_; // Vertex name or description
    double x_, y_; // Vertex position on 2D space
    size_t active_edge_count_; // Number of active edges
    std::vector<Edge *> edges_; // List of edges connected
    State state_; // Vertex state (NONE, ACTIVE, TESTING)

public:
    // Constructor
    Vertex(const uint64_t id, std::string name, const double x, const double y)
        : id_(id), name_(std::move(name)), x_(x), y_(y), active_edge_count_(0),
          edges_(), state_(State::NONE)
    {
    }

    [[nodiscard]] uint64_t GetId() const { return id_; };

    [[nodiscard]] const std::string &GetName() const { return name_; };

    [[nodiscard]] double GetX() const { return x_; };

    [[nodiscard]] double GetY() const { return y_; };

    [[nodiscard]] State GetState() const { return state_; };

    [[nodiscard]] size_t GetEdgeCount() const { return edges_.size(); };

    [[nodiscard]] Edge *GetEdge(const size_t index) const
    {
        return index < edges_.size() ? edges_[index] : nullptr;
    };

    [[nodiscard]] Vertex *GetAdjacent(const Edge *edge) const;

    [[nodiscard]] Edge *GetEdgeTo(const Vertex *v) const;

    [[nodiscard]] bool IsConnected(const Vertex *v) const;

    [[nodiscard]] size_t GetActiveEdgeCount() const { return active_edge_count_; };

    void ChangeActiveEdge(State current_state, State new_state);

    void SetState(const State state) { state_ = state; };

    void AddEdge(Edge *edge);

    ~Vertex();
};

#endif // VERTEX_H_
