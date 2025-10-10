/**
 * @file main.cpp
 * @brief Ponto de entrada principal para a aplicação de Gestão de Estoque.
 *
 * Nemertea: Algoritmo Heurístico de Exploração Adaptativa para o Problema do
 * Ciclo Hamiltoniano (PCH).
 * * OBJETIVO:
 * Encontrar um Ciclo Hamiltoniano (um caminho que visita todos os vértices exatamente uma vez,
 * retornando ao ponto de partida) em um grafo finito, não direcionado e geral.
 * * ESTRATÉGIA CENTRAL: Conquista Territorial
 * O algoritmo opera sob uma estratégia de 'conquista territorial'. Começando em um vértice
 * aleatório, ele expande progressivamente seu 'território' (Fronteira F) até que todos os
 * vértices sejam incluídos no caminho.
 * * MECANISMOS-CHAVE:
 * * 1. ESTADOS DINÂMICOS:
 * - O algoritmo utiliza estados internos (INACTIVE=0, TESTING=1, ACTIVE=2) em vértices e
 * arestas para guiar a navegação e evitar a exploração de elementos já conquistados (ACTIVE).
 * * 2. NBFS (Nemertea Best First Search):
 * - O NBFS é uma Busca em Largura (BFS) customizada, utilizada como motor de expansão.
 * - Ela busca o caminho mais curto para estender a Fronteira a partir do vértice ativo corrente.
 * - Usa um predicado estrito (SELECTCHILD) para filtrar caminhos, evitando retrocessos e a
 * formação prematura de ciclos.
 * * 3. ANÁLISE AMORTIZADA:
 * - Embora o NBFS possa ser chamado múltiplas vezes, a conquista permanente de vértices
 * (estado ACTIVE) garante que o espaço de busca diminua progressivamente.
 * - A complexidade é amortizada na prática para O(|V| + |E|), conferindo alta eficiência e
 * escalabilidade para grafos grandes, o principal diferencial do Nemertea.
 * * RETORNO:
 * TRUE se um Ciclo Hamiltoniano for encontrado (total de vértices conquistados = |V|) ou FALSE
 * caso contrário.
 *
 * @author Saulo Popov Zambiasi
 * @date 2025-10-09
 *
 * @section Dependencias
 * Requer a biblioteca 'Produto' para manipulação de itens e 'Estoque'
 * para operações de banco de dados.
 */

#include <iostream>
#include <chrono>
#include <cxxopts.hpp>
#include "graph.h"
#include "nemertea.h"

int main(const int argc, char *argv[])
{
    // Parameters inicialization

    cxxopts::Options options("nemertea", "Nemertea: A Territorial Expansion-Based Algorithm for the Hamiltonian Cycle Problem");

    options.add_options()
        ("g,graph", "Graph file (.json)", cxxopts::value<std::string>()->default_value(""))
        ("d,depth", "Max depht (between 3 and 20, default 5)", cxxopts::value<int>()->default_value("5"))
        ("t,type", "Operation type (cycle, path. Default: cycle)", cxxopts::value<std::string>()->default_value("cycle"))
        ("h,help", "Show this help message");

    std::string fname = "";
    size_t max_depth = 5;
    std::string type = "cycle";

    try
    {
        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            std::cout << options.help() << "\n";
            return 0;
        }

        fname = result["graph"].as<std::string>();
        max_depth = result["depth"].as<int>();
        type = result["type"].as<std::string>();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erro ao interpretar parâmetros: " << e.what() << "\n";
        std::cerr << "Use --help para ver as opções disponíveis.\n";
        return 1;
    }

    if (fname == "")
    {
        std::cout << "You need enter with a graph file.\n" << std::endl;
        std::cout << options.help() << "\n";
        return 1;
    }

    // Execution program

    const auto graph = new Graph(fname);
    graph->Load();
    const size_t vertex_count = graph->GetVertexCount();

    const auto inicial = std::chrono::high_resolution_clock::now();

    const size_t path_count = Nemertea(graph, max_depth, type == "cycle");

    const auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - inicial);


    // Printing results

    std::cout << "Nemertea execution " << duracao.count()
              << " microseconds. Found " << path_count
              << " of " << vertex_count << " vertices =>";

    const bool solved = path_count == vertex_count;
    if (solved)
        std::cout << " SOLVED." << std::endl;
    else
        std::cout << " INCONCLUSIVE." << std::endl;

    graph->Save(solved);

    delete graph;
    return 0;
}
