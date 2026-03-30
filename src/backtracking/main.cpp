//
// Hamiltonian Cycle — Backtracking (DFS exato)
//
// Referência clássica:
//   Ore, O. (1960). A note on Hamiltonian circuits.
//   American Mathematical Monthly, 67(1), 55.
//
// Uso:
//   backtracking <grafo.json> <saida.dot> [timeout_s]
//
//   timeout_s  (opcional, padrão 60) — abandona a busca após N segundos e
//              registra TIMEOUT no .dot; permite incluir o caso na tabela
//              do artigo como "não convergiu".
//

#include "../../src/common/graph.hpp"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

// ─────────────────────────────────────────────────────────────────────────────
//  Backtracking state
// ─────────────────────────────────────────────────────────────────────────────

struct BT {
    const Graph&           g;
    int                    n;          // number of vertices
    std::vector<int>       path;       // current path (vertex ids)
    std::vector<bool>      visited;    // visited[idx] = true if in path
    std::vector<int>       result;     // found HC (ids, closed), empty = none
    std::atomic<bool>&     timed_out;

    BT(const Graph& g_, std::atomic<bool>& to)
        : g(g_)
        , n(static_cast<int>(g_.vertices.size()))
        , visited(g_.vertices.size(), false)
        , timed_out(to)
    {
        path.reserve(n + 1);
    }

    // Returns true when a HC is found.
    bool dfs(int cur_id)
    {
        if (timed_out.load(std::memory_order_relaxed))
            return false;

        if (static_cast<int>(path.size()) == n) {
            // Try to close the cycle back to the starting vertex
            int start_id = path.front();
            for (int nb : g.adj.at(cur_id)) {
                if (nb == start_id) {
                    result = path;
                    result.push_back(start_id);
                    return true;
                }
            }
            return false;
        }

        // Iterate over neighbours ordered by degree ascending (Warnsdorff
        // pruning as a cheap speedup — still exact, just explores promising
        // branches first).
        auto it = g.adj.find(cur_id);
        if (it == g.adj.end()) return false;

        // Build candidate list with their unvisited-degree for ordering
        std::vector<std::pair<int,int>> cands; // (unvisited_degree, nb_id)
        cands.reserve(it->second.size());
        for (int nb_id : it->second) {
            int nb_idx = g.id_to_idx.at(nb_id);
            if (visited[nb_idx]) continue;

            // count unvisited neighbours of nb
            int deg = 0;
            auto nb_it = g.adj.find(nb_id);
            if (nb_it != g.adj.end())
                for (int nn : nb_it->second)
                    if (!visited[g.id_to_idx.at(nn)]) ++deg;

            cands.push_back({deg, nb_id});
        }

        // Sort ascending: visit lowest-degree first (Warnsdorff order)
        std::sort(cands.begin(), cands.end());

        for (auto& [deg, nb_id] : cands) {
            int nb_idx = g.id_to_idx.at(nb_id);
            visited[nb_idx] = true;
            path.push_back(nb_id);

            if (dfs(nb_id)) return true;

            path.pop_back();
            visited[nb_idx] = false;
        }

        return false;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────────────────────────────────────

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Uso: backtracking <grafo.json> <saida.dot> [timeout_s]\n";
        return 1;
    }

    std::string in_path  = argv[1];
    std::string out_path = argv[2];
    int         timeout_s = (argc >= 4) ? std::atoi(argv[3]) : 60;

    // ── Load (não cronometrado) ───────────────────────────────────────────────
    Graph g;
    try { g = load_graph(in_path); }
    catch (const std::exception& ex) {
        std::cerr << "Erro ao carregar grafo: " << ex.what() << "\n";
        return 1;
    }

    int n = static_cast<int>(g.vertices.size());
    std::cout << "Grafo: " << n << " vértices, "
              << g.edges.size() << " arestas\n";

    // ── Timeout watchdog ──────────────────────────────────────────────────────
    std::atomic<bool> timed_out{false};
    std::thread watchdog([&]() {
        std::this_thread::sleep_for(std::chrono::seconds(timeout_s));
        timed_out.store(true, std::memory_order_relaxed);
    });
    watchdog.detach();

    // ── Algoritmo (cronometrado) ──────────────────────────────────────────────
    BT bt(g, timed_out);

    // Tenta cada vértice como ponto de partida até encontrar um HC ou timeout.
    // Em grafos não-dirigidos, o HC existe independente do vértice inicial,
    // mas partir de vértices de grau baixo costuma convergir mais rápido.
    std::vector<int> start_order;
    start_order.reserve(n);
    for (auto& v : g.vertices) start_order.push_back(v.id);
    std::sort(start_order.begin(), start_order.end(), [&](int a, int b) {
        return g.adj.at(a).size() < g.adj.at(b).size();
    });

    auto t0 = Clock::now();

    for (int start_id : start_order) {
        if (timed_out.load()) break;
        if (!bt.result.empty()) break;

        int start_idx = g.id_to_idx.at(start_id);
        bt.path.clear();
        std::fill(bt.visited.begin(), bt.visited.end(), false);

        bt.visited[start_idx] = true;
        bt.path.push_back(start_id);
        bt.dfs(start_id);
    }

    auto t1 = Clock::now();
    long long elapsed_us = timed_out.load()
        ? -1LL
        : std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    // ── Resultado ─────────────────────────────────────────────────────────────
    if (!bt.result.empty()) {
        std::cout << "Ciclo hamiltoniano ENCONTRADO\n";
        std::cout << "Tempo: " << elapsed_us << " µs\n";
    } else if (timed_out.load()) {
        std::cout << "TIMEOUT após " << timeout_s << " s\n";
    } else {
        std::cout << "Nenhum ciclo hamiltoniano encontrado\n";
        std::cout << "Tempo: " << elapsed_us << " µs\n";
    }

    // ── Gravar DOT (não cronometrado) ─────────────────────────────────────────
    try {
        write_dot(out_path, in_path, "Backtracking (DFS + Warnsdorff order)",
                  g, elapsed_us, bt.result);
        std::cout << "DOT gravado em: " << out_path << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Erro ao gravar DOT: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
