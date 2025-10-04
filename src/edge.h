#ifndef EDGE_H_
#define EDGE_H_

#include <memory>
#include <vector>
#include "globals.h"

class Vertex;

class Edge
{
private:
  uint64_t id_;    // Edge id
  Vertex *a_, *b_; // This edge connects with 2 verticess
  int weight_;     // weight of connection
  State state_;    // Edge state (ACTIVE, NONE, TESTING)

public:
  Edge(uint64_t id, Vertex *a, Vertex *b, int weight)
      : id_(id), a_(a), b_(b), weight_(weight), state_(State::NONE) {};
  uint64_t GetId() const { return id_; };
  int GetWeight() const { return weight_; };
  void SetWeight(int weight) { weight_ = weight; };
  State GetState() const { return state_; };
  void SetState(State state);
  Vertex *GetA() const { return a_; };
  Vertex *GetB() const { return b_; };
  bool Contains(const Vertex *v) const { return v ? a_ == v || b_ == v : false; }
};

#endif // EDGE_H_