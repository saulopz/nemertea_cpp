#include "graph.h"
#include <iostream>
#include <chrono>
#include "nemertea.h"

int main(const int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0]
                << " <arquivo.json>" << std::endl;
        return 1;
    }
    const std::string fname = argv[1];

    size_t max_depth = 5;
    if (argc > 2)
    {
        try
        {
            max_depth = std::stoul(argv[2]);
        } catch (const std::invalid_argument &e)
        {
            std::cerr << "Erro: O segundo argumento ('" << argv[2]
                    << "') deve ser um número inteiro. : " << e.what() << std::endl;
            return 1;
        }
    }
    if (max_depth > 20)
    {
        std::cerr << "Erro: Profundidade máxima 20, usando o padrão 5"
                << std::endl;
        max_depth = 5;
    }

    const auto graph = new Graph(fname);
    graph->Load();
    const size_t vertex_count = graph->GetVertexCount();

    const auto inicial = std::chrono::high_resolution_clock::now();

    const size_t path_count = Nemertea(graph, max_depth);

    const auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - inicial);

    std::cout << "Nemertea execution " << duracao.count()
            << " microseconds. Vertex: " << vertex_count
            << " Path: " << path_count;

    const bool solved = path_count == vertex_count;
    if (solved)
        std::cout << " SOLVED." << std::endl;
    else
        std::cout << " INCONCLUSIVE." << std::endl;

    graph->Save(solved);
    delete graph;
    return 0;
}
