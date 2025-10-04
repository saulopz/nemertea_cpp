#ifndef NBFS_H_
#define NBFS_H_

#include <memory>
#include <utility>
#include "vertex.h"
#include "node.h"

class NBFS
{
private:
    Node *root_;                 // Origin node (NBFS is owner)
    std::vector<Node *> leaves_; // list of leaves
    bool first_;                 // If is first interaction
    size_t deepth_;              // Limit of deepth

    // Private methods
    std::pair<Node *, bool> SelectChild(Node *node, Edge *edge, bool first);
    size_t MakePath(Node *node);

public:
    NBFS(Vertex *startVertex, bool first, size_t deepth);
    size_t Run();
    ~NBFS();
};

#endif // NBFS_H_