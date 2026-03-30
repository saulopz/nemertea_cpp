#include "nbfs.hpp"
#include "graph.hpp"
#include "vertex.hpp"
#include <cstdlib>

size_t NBFS::Run(Vertex *startVertex, bool first, size_t depth)
{
    while (!leaves_.empty()) // Resetting leaves
        leaves_.pop();

    search_id_++; // New search id
    root_ = startVertex;
    root_->AddToSearch(nullptr, search_id_);
    root_->SetState(State::ACTIVE);
    leaves_.push(root_);
    size_t level = 0;
    while (!leaves_.empty() && level < depth)
    {
        // It retrieves the number of nodes at that level so we know
        // when we are changing levels. Inside next loop new nodes
        // can be insert, but it is for next level.

        size_t nodes_at_this_level = leaves_.size();

        for (size_t i = 0; i < nodes_at_this_level; i++)
        {
            auto vertex = leaves_.front();
            leaves_.pop();

            auto neighbor_count = vertex->GetNeighborsCount();
            for (size_t i = 0; i < neighbor_count; i++)
            {
                auto target = vertex->GetNeighbor(i);
                auto [child, found] = SelectChild(vertex, target, first);
                if (found)
                    return MakePath(child);
                else if (child)
                    leaves_.push(child);
            }
        }
        level++;
    }
    return 0;
}

std::pair<Vertex *, bool> NBFS::SelectChild(Vertex *vertex, Vertex *target, bool first) const
{
    const auto vertex_id = vertex->GetId();
    const auto target_id = target->GetId();
    const auto root_id = root_->GetId();

    // Case 1: Edge already active - ignore
    if (graph_->GetConnectionState(vertex_id, target_id) == State::ACTIVE)
        return {nullptr, false};

    if (target->GetState() != State::ACTIVE && target->IsTesting(search_id_))
    {
        // Case 2: General blocking of TESTING outside of the first iteration.
        if (!first)
            return {nullptr, false};

        // Case 3: Ancestor locking in the tree during only the "first iteration".
        //         Verify if it's ancestor.
        auto local = vertex->GetParent();
        while (local)
        {
            if (local->GetId() == target_id)
            {
                return {nullptr, false};
            }
            local = local->GetParent();
        }
        // Not ancestor. Let's pass to the next case.
    }

    // Case 4: Ignore returning to father
    auto parent = vertex->GetParent();
    if (parent && target_id == parent->GetId())
        return {nullptr, false};

    // Case 5: If it is root with no active connections yet
    if (target_id == root_id && root_->GetActiveConnectionsCount() == 0)
    {
        target->AddToSearch(vertex, search_id_);
        return {target, true};
    }

    // Case 6 and 7: If vertex is active and is neighbor of root, path found or ignored
    if (target->GetState() == State::ACTIVE)
    {
        // Case 6: vertex is active and it's a root neighbor, than i found target
        auto target_to_root = graph_->GetConnectionState(target_id, root_id);
        if (target_to_root == State::ACTIVE)
        {
            target->AddToSearch(vertex, search_id_);
            return {target, true};
        }
        // Case 7: The adjacent vertex is ACTIVE. It's alread in frontier and it's not valid.
        return {nullptr, false};
    }

    // Case 8: Free vertex, add as child
    graph_->SetConnectionState(vertex_id, target_id, State::TESTING);
    target->AddToSearch(vertex, search_id_); // Set target as TESTING
    target->SetState(State::TESTING);
    return {target, false};
}

size_t NBFS::MakePath(Vertex *vertex) const
{
    // If there is an active connection to the root,
    // disconnect it to open access to the new region.
    const auto vertex_id = vertex->GetId();
    const auto root_id = root_->GetId();
    if (vertex->GetState() == State::ACTIVE && vertex_id != root_id)
        if (graph_->GetConnectionState(root_id, vertex_id) == State::ACTIVE)
            graph_->SetConnectionState(root_id, vertex_id, State::INACTIVE);

    // Returns to the root and activates the vertices and edges along the path.
    size_t new_vertex_count = 0;
    auto local = vertex;
    do
    {
        if (local->GetState() != State::ACTIVE)
        {
            local->SetState(State::ACTIVE);
            new_vertex_count++;
        }

        // Change connection to parent as ACTIVE
        if (auto parent = local->GetParent())
        {
            graph_->SetConnectionState(local->GetId(), parent->GetId(), State::ACTIVE);
            local = parent; // Go to parent
        }
        else
        {
            break;
        }

    } while (local->GetId() != root_id);

    return new_vertex_count;
}