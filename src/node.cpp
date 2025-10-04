#include "node.h"
#include "globals.h"

Node *Node::AddChild(Vertex *vertex)
{
    if (!vertex)
        return nullptr;
    auto child = new Node(vertex);
    child->parent_ = this;
    child->edge_ = vertex_->GetEdgeTo(vertex);
    children_.push_back(child);
    return child;
}

Node::~Node()
{
    if (edge_ && edge_->GetState() != State::ACTIVE)
        edge_->SetState(State::NONE);
    if (vertex_ && vertex_->GetState() != State::ACTIVE)
        vertex_->SetState(State::NONE);
    for (auto child : children_)
        delete child;
    children_.clear();
}