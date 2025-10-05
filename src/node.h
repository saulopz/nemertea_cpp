#ifndef NODE_H_
#define NODE_H_

#include "vertex.h"

class Node
{
private:
    Vertex *vertex_; // Vertex of Node
    Node *parent_; // Node parent
    Edge *edge_; // Edge to parent
    std::vector<Node *> children_; // List of children

public:
    explicit Node(Vertex *vertex)
        : vertex_(vertex), parent_(), edge_(), children_()
    {
    }

    Node *AddChild(Vertex *vertex);

    [[nodiscard]] Vertex *GetVertex() const { return vertex_; }

    [[nodiscard]] Node *GetParent() const { return parent_; }

    [[nodiscard]] Edge *GetEdge() const { return edge_; }

    ~Node();
};

#endif // NODE_H_
