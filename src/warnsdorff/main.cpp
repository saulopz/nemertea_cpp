//
// Hamiltonian Cycle — Heurística de Warnsdorff
//
// Reference:
//   Warnsdorff, H. C. (1823). Des Rösselsprunges einfachste und allgemeinste
//   Lösung. Schmalkalden.
//   Pohl, I. (1967). A method for finding Hamilton paths and Knight's tours.
//   Communications of the ACM, 10(7), 446–449.
//
// Strategy:
//   At each step, it chooses the unvisited neighbor with the LOWEST out-degree
//   (number of neighbors not yet visited). This tends to avoid dead ends,
//   leaving well-connected vertices for the end. It is heuristic: it does not
//   guarantee finding the HC even if it exists.
//   When it fails to close the loop, it restarts from another vertex.
//
// Usage:
//   warnsdorff <grafo.json> <saida.dot>
//

#include "../../src/common/graph.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

// ─────────────────────────────────────────────────────────────────────────────
//  Warnsdorff: an attempt starting from an initial vertex
// ─────────────────────────────────────────────────────────────────────────────

// Returns the closed path (ids) if HC was found, empty otherwise.
static std::vector<int> warnsdorff_try(const Graph &g, int start_id)
{
    int n = static_cast<int>(g.vertices.size());

    std::vector<bool> visited(g.vertices.size(), false);
    std::vector<int> path;
    path.reserve(n + 1);

    auto mark = [&](int id) { visited[g.id_to_idx.at(id)] = true; };
    auto is_visited = [&](int id) -> bool { return visited[g.id_to_idx.at(id)]; };

    // Counts unvisited neighbors of a vertex.
    auto unvisited_degree = [&](int id) -> int {
        int deg = 0;
        auto it = g.adj.find(id);
        if (it == g.adj.end())
            return 0;
        for (int nb : it->second)
            if (!is_visited(nb))
                ++deg;
        return deg;
    };

    mark(start_id);
    path.push_back(start_id);
    int cur = start_id;

    while (static_cast<int>(path.size()) < n)
    {
        auto it = g.adj.find(cur);
        if (it == g.adj.end())
            return {};

        // Choose the unvisited neighbor with the lowest degree of exit (Warnsdorff).
        int best_id = -1;
        int best_deg = std::numeric_limits<int>::max();

        for (int nb : it->second)
        {
            if (is_visited(nb))
                continue;
            int d = unvisited_degree(nb);
            if (d < best_deg)
            {
                best_deg = d;
                best_id = nb;
            }
        }

        if (best_id == -1)
            return {}; // dead end

        mark(best_id);
        path.push_back(best_id);
        cur = best_id;
    }

    // Try to close the cycle.
    for (int nb : g.adj.at(cur))
        if (nb == start_id)
        {
            path.push_back(start_id);
            return path;
        }

    return {}; // It didn't close.
}

// ─────────────────────────────────────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────────────────────────────────────

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Uses: warnsdorff <grafo.json> <saida.dot>\n";
        return 1;
    }

    std::string in_path = argv[1];
    std::string out_path = argv[2];

    // ── Load (não cronometrado) ───────────────────────────────────────────────
    Graph g;
    try
    {
        g = load_graph(in_path);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error loading graph: " << ex.what() << "\n";
        return 1;
    }

    int n = static_cast<int>(g.vertices.size());
    std::cout << "Graph: " << n << " vrtices, " << g.edges.size() << " edges\n";

    // ── Algorithm (timed) ──────────────────────────────────────────────
    //
    // Try each vertex as a starting point, in order of increasing degree.
    // When there is a tie in the Warnsdorff criterion, the first candidate
    // in the adjacency list is chosen, deterministic and reproducible.

    std::vector<int> start_order;
    start_order.reserve(n);
    for (auto &v : g.vertices)
        start_order.push_back(v.id);
    std::sort(start_order.begin(), start_order.end(),
              [&](int a, int b) { return g.adj.at(a).size() < g.adj.at(b).size(); });

    std::vector<int> result;

    auto t0 = Clock::now();

    for (int start_id : start_order)
    {
        result = warnsdorff_try(g, start_id);
        if (!result.empty())
            break;
    }

    auto t1 = Clock::now();
    long long elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    // ── Result ─────────────────────────────────────────────────────────────
    if (!result.empty())
    {
        std::cout << "Hamiltonian cycle FOUND\n";
        std::cout << "Time: " << elapsed_us << " µs\n";
    }
    else
    {
        std::cout << "No Hamiltonian cycle found. "
                     "(Heuristics do not guarantee completeness.)\n";
        std::cout << "Time: " << elapsed_us << " µs\n";
    }

    // ── Gravar DOT (não cronometrado) ─────────────────────────────────────────
    try
    {
        write_dot(out_path, in_path, "Warnsdorff", g, elapsed_us, result);
        std::cout << "DOT recorded in: " << out_path << "\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error recording DOT: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
