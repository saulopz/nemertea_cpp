#include <iostream>
#include <vector>
#include <cstdint>
#include <limits>
#include <unordered_map>
#include <bitset>
#include <string>
#include <algorithm>
#include "vertex.h"
#include "graph.h"

constexpr int INF = std::numeric_limits<int>::max();

// --- Supondo que as classes Vertex, Edge, Graph já existam ---
// Vertex tem GetId(), GetEdgeTo(Vertex* v)
// Graph tem vertices_ e edges_

class HeldKarp
{
private:
    const std::vector<Vertex *> &verts;
    size_t n;
    std::unordered_map<uint64_t, size_t> vertex_index;
    std::vector<std::vector<int>> adj;

public:
    HeldKarp(const std::vector<Vertex *> &vertices)
        : verts(vertices), n(vertices.size())
    {
        // Criar mapa id -> índice (0..n-1) para usar bitmask
        for (size_t i = 0; i < n; ++i)
            vertex_index[verts[i]->GetId()] = i;

        // Construir matriz de adjacência 0/1
        adj.assign(n, std::vector<int>(n, 0));
        for (size_t i = 0; i < n; ++i)
        {
            Vertex *u = verts[i];
            for (size_t j = 0; j < n; ++j)
            {
                if (i == j)
                    continue;
                Vertex *v = verts[j];
                if (u->GetEdgeTo(v) != nullptr)
                    adj[i][j] = 1;
            }
        }
    }

    // Retorna true se existir ciclo Hamiltoniano
    bool HasHamiltonianCycle()
    {
        if (n == 0)
            return false;
        if (n == 1)
            return verts[0]->GetEdgeCount() > 0;

        int full_mask = (1 << n) - 1;
        // dp[mask][i] = true se existe caminho que visita subset mask e termina em i
        std::vector<std::vector<bool>> dp(1 << n, std::vector<bool>(n, false));

        // Inicialização: caminhos com apenas o vértice 0
        dp[1 << 0][0] = true;

        for (auto mask = 1; mask <= full_mask; ++mask)
        {
            for (auto u = 0; u < n; ++u)
            {
                if (!(mask & (1 << u)) || !dp[mask][u])
                    continue;
                // tentar expandir para v
                for (int v = 0; v < n; ++v)
                {
                    if (mask & (1 << v))
                        continue; // já visitado
                    if (adj[u][v])
                        dp[mask | (1 << v)][v] = true;
                }
            }
        }

        // verificar se existe caminho que termina em algum vizinho de 0
        for (auto v = 1; v < n; ++v)
        {
            if (dp[full_mask][v] && adj[v][0])
                return true;
        }
        return false;
    }
};
