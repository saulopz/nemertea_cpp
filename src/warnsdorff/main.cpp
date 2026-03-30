//
// Hamiltonian Cycle — Heurística de Warnsdorff
//
// Referência:
//   Warnsdorff, H. C. (1823). Des Rösselsprunges einfachste und allgemeinste
//   Lösung. Schmalkalden.
//   Pohl, I. (1967). A method for finding Hamilton paths and Knight's tours.
//   Communications of the ACM, 10(7), 446–449.
//
// Estratégia:
//   A cada passo, escolhe o vizinho não visitado com MENOR grau de saída
//   (número de vizinhos ainda não visitados). Isso tende a evitar becos sem
//   saída deixando vértices bem conectados para o final.
//   É heurístico: não garante encontrar o HC mesmo que ele exista.
//   Quando falha no fechamento do ciclo, reinicia a partir de outro vértice.
//
// Uso:
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
//  Warnsdorff: uma tentativa a partir de um vértice inicial
// ─────────────────────────────────────────────────────────────────────────────

// Retorna o caminho (ids) fechado se encontrou HC, vazio caso contrário.
static std::vector<int> warnsdorff_try(const Graph &g, int start_id)
{
    int n = static_cast<int>(g.vertices.size());

    std::vector<bool> visited(g.vertices.size(), false);
    std::vector<int> path;
    path.reserve(n + 1);

    auto mark = [&](int id) { visited[g.id_to_idx.at(id)] = true; };
    auto is_visited = [&](int id) -> bool { return visited[g.id_to_idx.at(id)]; };

    // Conta vizinhos não visitados de um vértice
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

        // Escolhe o vizinho não visitado com menor grau de saída (Warnsdorff)
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
            return {}; // beco sem saída

        mark(best_id);
        path.push_back(best_id);
        cur = best_id;
    }

    // Tenta fechar o ciclo
    for (int nb : g.adj.at(cur))
        if (nb == start_id)
        {
            path.push_back(start_id);
            return path;
        }

    return {}; // não fechou
}

// ─────────────────────────────────────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────────────────────────────────────

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Uso: warnsdorff <grafo.json> <saida.dot>\n";
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
        std::cerr << "Erro ao carregar grafo: " << ex.what() << "\n";
        return 1;
    }

    int n = static_cast<int>(g.vertices.size());
    std::cout << "Grafo: " << n << " vértices, " << g.edges.size() << " arestas\n";

    // ── Algoritmo (cronometrado) ──────────────────────────────────────────────
    //
    // Tenta cada vértice como ponto de partida, na ordem de grau crescente.
    // Quando há empate no critério de Warnsdorff, o primeiro candidato na
    // lista de adjacência é escolhido — determinístico e reproduzível.

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

    // ── Resultado ─────────────────────────────────────────────────────────────
    if (!result.empty())
    {
        std::cout << "Ciclo hamiltoniano ENCONTRADO\n";
        std::cout << "Tempo: " << elapsed_us << " µs\n";
    }
    else
    {
        std::cout << "Nenhum ciclo hamiltoniano encontrado "
                     "(heurística não garante completude)\n";
        std::cout << "Tempo: " << elapsed_us << " µs\n";
    }

    // ── Gravar DOT (não cronometrado) ─────────────────────────────────────────
    try
    {
        write_dot(out_path, in_path, "Warnsdorff", g, elapsed_us, result);
        std::cout << "DOT gravado em: " << out_path << "\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Erro ao gravar DOT: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
