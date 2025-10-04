#include "graph.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include "nemertea.h"
#include "heldkarp.h"
#include "dfs.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0]
                  << " <arquivo.json>" << std::endl;
        return 1;
    }
    std::string fname = argv[1];

    size_t max_depth = 5;
    if (argc > 2)
    {
        try
        {
            max_depth = std::stoul(argv[2]);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Erro: O segundo argumento ('" << argv[2]
                      << "') deve ser um número inteiro." << std::endl;
            return 1;
        }
    }
    if (max_depth > 20)
    {
        std::cerr << "Erro: Profundidade máxima 20, usando o padrão 5"
                  << std::endl;
        max_depth = 5;
    }

    std::srand(std::time(0));
    auto graph = new Graph(fname);
    graph->Load();
    size_t vertex_count = graph->GetVertexCount();

    auto inicial = std::chrono::high_resolution_clock::now();

    size_t path_count = Nemertea(graph, max_depth);
    //size_t path_count = 0;

    // HeldKarp solver(graph->GetVertices()); // usa vetor de vértices
    // bool exists = solver.HasHamiltonianCycle();

    //HamiltonianDFS solver(graph->GetVertices());
    //bool exists = solver.HasHamiltonianCycle();

    bool exists = false;

    if (exists)
        std::cout << "Ciclo Hamiltoniano existe!\n";
    else
        std::cout << "Não existe ciclo Hamiltoniano.\n";

    auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - inicial);

    std::cout << "Nemertea execution " << duracao.count()
              << " microseconds. Vertex: " << vertex_count
              << " Path: " << path_count;

    bool solved = path_count == vertex_count;
    if (solved)
        std::cout << " SOLVED." << std::endl;
    else
        std::cout << " INCONCLUSIVE." << std::endl;

    graph->Save(solved);
    delete graph;
    return 0;
}