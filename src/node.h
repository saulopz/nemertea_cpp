#ifndef NODE_H_
#define NODE_H_

#include <memory>
#include "vertex.h"

class Node
{
private:
    Vertex *vertex_;               // Vertex of Node
    Node *parent_;                 // Node parent
    Edge *edge_;                   // Edge to parent
    std::vector<Node *> children_; // List of children

public:
    Node(Vertex *vertex)
        : vertex_(vertex), parent_(), edge_(), children_() {}
    Node *AddChild(Vertex *vertex);
    Vertex *GetVertex() { return vertex_; }
    Node *GetParent() { return parent_; }
    Edge *GetEdge() { return edge_; }
    ~Node();
};

#endif // NODE_H_