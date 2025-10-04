#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "vertex.h"
#include "graph.h"

// --- Supondo que as classes Vertex, Edge, Graph já existam ---

class HamiltonianDFS {
public:
    HamiltonianDFS(const std::vector<Vertex*>& vertices)
        : verts(vertices), n(vertices.size()) {}

    // Retorna true se existe ciclo Hamiltoniano
    bool HasHamiltonianCycle() {
        if (n == 0) return false;
        visited.assign(n, false);

        // Começamos do vértice 0
        path.clear();
        path.push_back(0);
        visited[0] = true;

        return DFS(0, 1);
    }

private:
    const std::vector<Vertex*>& verts;
    size_t n;
    std::vector<bool> visited;
    std::vector<int> path; // indices dos vértices na ordem visitada

    // DFS recursivo
    bool DFS(int u, int depth) {
        if (depth == n) {
            // Se o último vértice está conectado ao primeiro, temos ciclo
            for (size_t i = 0; i < verts[u]->GetEdgeCount(); ++i) {
                Vertex* v = verts[u]->GetAdjacent(verts[u]->GetEdge(i));
                if (v == verts[path[0]]) return true;
            }
            return false;
        }

        // Explorar vizinhos do vértice u
        for (size_t i = 0; i < verts[u]->GetEdgeCount(); ++i) {
            Vertex* v = verts[u]->GetAdjacent(verts[u]->GetEdge(i));
            if (!v) continue;
            int vidx = GetIndex(v);
            if (visited[vidx]) continue;

            // Poda: se este vizinho já foi visitado, não seguimos
            visited[vidx] = true;
            path.push_back(vidx);

            if (DFS(vidx, depth + 1)) return true; // encontrou ciclo, retorna

            // Backtrack
            visited[vidx] = false;
            path.pop_back();
        }

        return false; // nenhum caminho válido encontrado
    }

    // Retorna índice do vértice no vetor verts
    int GetIndex(Vertex* v) {
        for (size_t i = 0; i < verts.size(); ++i)
            if (verts[i] == v) return i;
        return -1; // não deveria acontecer
    }
};