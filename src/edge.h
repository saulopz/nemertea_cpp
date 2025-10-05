#ifndef EDGE_H_
#define EDGE_H_

#include <memory>
#include "globals.h"

class Vertex;

class Edge
{
private:
    uint64_t id_; // Edge id
    Vertex *a_, *b_; // This edge connects with 2 vertices
    int weight_; // weight of connection
    State state_; // Edge state (ACTIVE, NONE, TESTING)

public:
    Edge(const uint64_t id, Vertex *a, Vertex *b, const int weight)
        : id_(id), a_(a), b_(b), weight_(weight), state_(State::NONE)
    {
    };

    [[nodiscard]] uint64_t GetId() const { return id_; };

    [[nodiscard]] int GetWeight() const { return weight_; };

    void SetWeight(const int weight) { weight_ = weight; };

    [[nodiscard]] State GetState() const { return state_; };

    void SetState(State state);

    [[nodiscard]] Vertex *GetA() const { return a_; };

    [[nodiscard]] Vertex *GetB() const { return b_; };

    bool Contains(const Vertex *v) const
    {
        return v ? a_ == v || b_ == v : false;
    }
};

#endif // EDGE_H_
