#include "vertex.h"

Vertex *Vertex::GetAdjacent(const Edge *edge) const
{
    auto a = edge->GetA();
    auto b = edge->GetB();
    if (a->id_ == id_)
        return b;
    if (b->id_ == id_)
        return a;
    return nullptr;
}

void Vertex::AddEdge(Edge *edge)
{
    edges_.push_back(edge);
};

Edge *Vertex::GetEdgeTo(const Vertex *v) const
{
    if (!v)
        return nullptr;
    for (const auto &edge : edges_)
        if (edge->Contains(v))
            return edge;
    return nullptr;
}

bool Vertex::IsConnected(const Vertex *v) const
{
    return GetEdgeTo(v) ? true : false;
}

void Vertex::ChangeActiveEdge(State current_state, State new_state)
{
    if (current_state != State::ACTIVE && new_state == State::ACTIVE)
    {
        active_edge_count_++;
        return;
    }
    if (current_state == State::ACTIVE && new_state != State::ACTIVE)
        active_edge_count_--;
}

Vertex::~Vertex()
{
    edges_.clear();
}
