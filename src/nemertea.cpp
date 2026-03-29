#include "nemertea.hpp"
#include "nbfs.hpp"

// size_t FirstPath(Graph *graph, Vertex *root);

size_t Nemertea::Run(const size_t depth, const bool cycle) const
{
    auto nbfs = NBFS(graph_);
    Vertex *prev = nullptr;                             // Previous vertex, starts null
    const auto vertex_count = graph_->GetVertexCount(); // Number of vertices in the graph
    auto first_vertex = graph_->GetRandomVertex();      // Take a random vertex
    const auto first_vertex_id = first_vertex->GetId(); // Get its id
    first_vertex->SetState(State::ACTIVE);              // The first vertex is ACTIVE
    auto current = first_vertex;                        // Navigation vertex
    size_t path_count = 1;                              // The first vertex is already on the path
    bool first = true;

    // If is a path resolution. Create a first line and then start Nemertea in the root point
    if (!cycle)
    {
        path_count += FirstPath(first_vertex);
        first = false;
    }

    do
    {
        size_t size = 0;
        do
        {
            size = nbfs.Run(current->GetId(), first, depth);
            path_count += size;
            first = false;

        } while (size > 0);

        const auto next = NextVertex(prev, current);
        prev = current;
        current = next;

    } while (current && current->GetId() != first_vertex_id && path_count < vertex_count);

    // If the current id is equal to the first vertex id, return to the starting vertex - finish
    // if pathCount is equal to vertexCount, all vertices are active - finish

    return path_count;
}

Vertex *Nemertea::NextVertex(const Vertex *prev, const Vertex *current) const
{
    const auto neighbor_count = current->GetNeighborsCount();
    for (size_t i = 0; i < neighbor_count; i++)
    {
        auto neighbor = current->GetNeighbor(i);
        if (graph_->GetConnectionState(current->GetId(), neighbor->GetId()) == State::ACTIVE)
        {
            if (!prev || neighbor->GetId() != prev->GetId())
                return neighbor;
        }
    }
    return nullptr;
}

size_t Nemertea::FirstPath(Vertex *root) const
{
    size_t count = 0;
    auto vertex = root;
    bool found = false;
    do
    {
        found = false;
        size_t neighbor_count = vertex->GetNeighborsCount();
        size_t i = 0;
        while (i < neighbor_count && !found)
        {
            auto neighbor = vertex->GetNeighbor(i);
            if (neighbor->GetState() != State::ACTIVE)
            {
                graph_->SetConnectionState(vertex->GetId(), neighbor->GetId(), State::ACTIVE);
                neighbor->SetState(State::ACTIVE);
                count++;
                vertex = neighbor;
                found = true;
            }
            i++;
        }
    } while (found);
    return count;
}
