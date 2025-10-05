#include "nemertea.h"
#include "nbfs.h"

size_t Nemertea(Graph *graph, const size_t depth)
{
    Vertex *prev = nullptr;                             // Previous vertex, starts null
    const auto vertex_count = graph->GetVertexCount();  // Number of vertices in the graph
    const auto first_vertex = graph->GetRandomVertex(); // Take a random vertex
    const auto first_vertex_id = first_vertex->GetId(); // Get its id
    first_vertex->SetState(State::ACTIVE);              // The first vertex is ACTIVE
    auto current = first_vertex;                        // Navigation vertex
    size_t path_count = 1;                              // The first vertex is already on the path
    bool first = true;                                  // First region is being created

    do
    {
        size_t size = 0;
        do
        {
            auto nbfs = NBFS(current, first, depth);
            size = nbfs.Run();
            path_count += size;
            first = false;
        } while (size > 0);

        const auto next = NextVertex(prev, current);
        prev = current;
        current = next;
    } while (current && current->GetId() != first_vertex_id &&
             path_count < vertex_count);

    // If the current id is equal to the first vertex id, return to the starting vertex - finish
    // if pathCount is equal to vertexCount, all vertices are active - finish

    return path_count;
}

Vertex *NextVertex(const Vertex *prev, const Vertex *current)
{
    const auto prev_id = prev ? prev->GetId() : 0;
    const auto edge_count = current->GetEdgeCount();
    for (size_t i = 0; i < edge_count; i++)
    {
        auto e = current->GetEdge(i);
        if (e && e->GetState() == State::ACTIVE)
        {
            auto adjacent = current->GetAdjacent(e);
            if (!prev || adjacent->GetId() != prev_id)
                return adjacent;
        }
    }
    return nullptr;
}
