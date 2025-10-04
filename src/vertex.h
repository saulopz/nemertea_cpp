#ifndef VERTEX_H_
#define VERTEX_H_

#include "edge.h"
#include <vector>
#include <string>

class Vertex
{
private:
  uint64_t id_;               // Vertex id
  std::string name_;          // Vertex name or description
  double x_, y_;              // Vertex position on 2D space
  size_t active_edge_count_;  // Number of active edges
  std::vector<Edge *> edges_; // List of edges connected
  State state_;               // Vertex state (NONE, ACTIVE, TESTING)

public:
  // Constructor
  Vertex(uint64_t id, const std::string &name, double x, double y)
      : id_(id), name_(name), x_(x), y_(y), active_edge_count_(0),
        edges_(), state_(State::NONE) {}

  // Getters
  uint64_t GetId() const { return id_; };
  const std::string &GetName() const { return name_; };
  double GetX() const { return x_; };
  double GetY() const { return y_; };
  State GetState() const { return state_; };
  size_t GetEdgeCount() const { return edges_.size(); };
  Edge *GetEdge(size_t index) const { return index < edges_.size() ? edges_[index] : nullptr; };
  Vertex *GetAdjacent(const Edge *edge) const;
  Edge *GetEdgeTo(const Vertex *v) const;
  size_t GetActiveEdgeCount() const { return active_edge_count_; };
  void ChangeActiveEdge(State current_state, State new_state);

  // Setters
  void SetState(State state) { state_ = state; };
  void AddEdge(Edge *edge);

  // Verificadores
  bool IsConnected(const Vertex *v) const;

  ~Vertex();
};

#endif // VERTEX_H_