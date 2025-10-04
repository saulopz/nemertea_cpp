#include "graph.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

/** returns a vertex by it's id. It's used only for loadling opperation.
 * It is a O(n) operation.
 *
 * Parameters
 * ----------
 * - id (uint64_t): vertex id to find it.
 *
 * Returns
 * -------
 * - Vertex*
 */
Vertex *Graph::GetVertexById(uint64_t id)
{
    for (const auto &vertex : vertices_)
        if (vertex->GetId() == id)
            return vertex;
    return nullptr;
}

void Graph::Load()
{
    std::ifstream file(filename_);
    if (!file.is_open())
    {
        std::cerr << "Error opening file " << filename_ << std::endl;
        return;
    }
    json data;
    try
    {
        file >> data;
    }
    catch (json::parse_error &e)
    {
        std::cerr << "Erro no parsing do JSON: " << e.what() << std::endl;
        return;
    }

    // Load vertices
    if (data.contains("vertex") && data["vertex"].is_array())
    {
        for (const auto &v : data["vertex"])
        {
            if (v.contains("id") && v.contains("name") &&
                v.contains("x") && v.contains("y"))
            {
                uint64_t id = v["id"];
                std::string name = v["name"];
                double x = v["x"];
                double y = v["y"];
                vertices_.push_back(new Vertex(id, name, x / 10.0, y / 10.0));
            }
        }
    }

    // Load edges
    if (data.contains("edge") && data["edge"].is_array())
    {
        for (const auto &e : data["edge"])
        {
            if (e.contains("id") && e.contains("a") &&
                e.contains("b") && e.contains("weight"))
            {
                uint64_t id = e["id"];
                uint64_t a_id = e["a"];
                uint64_t b_id = e["b"];
                int weight = e["weight"];

                auto a_vertex = GetVertexById(a_id);
                auto b_vertex = GetVertexById(b_id);

                if (a_vertex && b_vertex)
                {
                    auto edge = new Edge(id, a_vertex, b_vertex, weight);
                    edges_.push_back(edge);
                    a_vertex->AddEdge(edge);
                    b_vertex->AddEdge(edge);
                }
            }
        }
    }
}

void Graph::Save(bool solved)
{
    // Implement saving the graph to a dot file
    std::stringstream ss;
    ss << "// Graph name: " << filename_ << "\n";
    ss << "// Algoirthm : Nemertea\n";
    ss << "// Run time  : \n";
    ss << "// Date      : \n";
    ss << "graph G {\n";
    ss << "\tlayout=neato;\n";
    ss << "\toverlap=false;\n";
    ss << "\tsplines=true;\n";
    if (!solved)
        ss << "\tbgcolor=OldLace;\n";
    ss << "\tnode [pin=true];\n";
    ss << "\tnode [shape=circle, width=1.0, height=1.0, fixedsize=true, style=filled, fillcolor=\"lightblue\"];\n\n";

    // Get vertices
    for (const auto &vertex : vertices_)
    {
        ss << "\tV" << vertex->GetId();
        ss << " [label=\"" << vertex->GetName() << "\", style=filled,";
        ss << " pos=\"" << vertex->GetX() << "," << vertex->GetY() << "!\",";

        if (vertex->GetState() == State::ACTIVE)
            ss << " fillcolor=\"black\", color=\"black\"];\n";
        else
            ss << " fillcolor=\"blue\", color=\"blue\"];\n";
    }
    ss << "\n";

    // Get edges
    for (const auto &edge : edges_)
    {
        ss << "\tV" << edge->GetA()->GetId();
        ss << " -- V" << edge->GetB()->GetId();
        if (edge->GetState() == State::ACTIVE)
            ss << " [color=\"black\", penwidth=50.0];\n";
        else
            ss << " [color=\"blue\", penwidth=10.0];\n";
    }
    ss << "}\n";

    std::string dot_content = ss.str();

    // Saving the string in a dot file

    std::filesystem::path filename_dot{filename_};

    std::ofstream dot_file(filename_dot.replace_extension(".dot"));
    if (!dot_file.is_open())
    {
        std::cerr << "Erro ao criar o arquivo "
                  << filename_dot.replace_extension(".dot")
                  << std::endl;
        return;
    }
    dot_file << dot_content;
    dot_file.close();
}

Graph::~Graph()
{
    for (const auto &vertex : vertices_)
        delete vertex;
    for (const auto &edge : edges_)
        delete edge;
    vertices_.clear();
    edges_.clear();
}
