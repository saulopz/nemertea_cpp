#ifndef NBFS_H_
#define NBFS_H_

#include <memory>
#include <utility>
#include "vertex.h"
#include "node.h"

class NBFS
{
private:
    Node *root_; // Origin node (NBFS is owner)
    std::vector<Node *> leaves_; // list of leaves
    bool first_; // If is first interaction
    size_t depth_; // Limit of depth

    // Private methods
    std::pair<Node *, bool> SelectChild(Node *node, Edge *edge, bool first) const;

    size_t MakePath(const Node *node) const;

public:
    NBFS(Vertex *startVertex, bool first, size_t depth);

    size_t Run();

    ~NBFS();
};

#endif // NBFS_H_
