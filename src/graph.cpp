#include "graph.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Vertex *Graph::GetRandomVertex()
{
    if (vertices_.empty())
        return nullptr;
    const size_t random_index = index_distribution_(random_engine_);
    return vertices_[random_index];
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
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return;
    }

    // Load vertices
    if (data.contains("vertex") && data["vertex"].is_array())
    {
        for (const auto &v : data["vertex"])
        {
            if (v.contains("id") && v.contains("name") && v.contains("x") && v.contains("y"))
            {
                uint64_t id = v["id"];
                std::string name = v["name"];
                double x = v["x"];
                double y = v["y"];
                vertices_.push_back(new Vertex(id, name, x / 10.0, y / 10.0));
            }
        }
    }
    vector_size_ = vertices_.size();
    adj_.assign(vector_size_ * vector_size_, State::NONE);

    // Load edges
    if (data.contains("edge") && data["edge"].is_array())
    {
        for (const auto &e : data["edge"])
        {
            if (e.contains("id") && e.contains("a") && e.contains("b") && e.contains("weight"))
            {
                uint64_t a_id = e["a"];
                uint64_t b_id = e["b"];
                // int weight = e["weight"]; // Not necessary to Nemertea now

                vertices_[a_id]->Connect(vertices_[b_id]);
                vertices_[b_id]->Connect(vertices_[a_id]);
                SetAdjacent(a_id, b_id);
            }
        }
    }

    if (!vertices_.empty())
    {
        index_distribution_ = std::uniform_int_distribution<size_t>(0, vertices_.size() - 1);
    }
}

void Graph::Save(bool solved)
{
    // Implement saving the graph to a dot file
    std::stringstream ss;
    ss << "// Graph name: " << filename_ << "\n";
    ss << "// Algorithm : Nemertea\n";
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

    size_t n = vector_size_;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = i + 1; j < n; j++)
        {
            if (adj_[i * n + j] != State::NONE)
            {
                ss << "\tV" << i;   // Vector ID a
                ss << " -- V" << j; // Vector ID b
                if (adj_[i * n + j] == State::ACTIVE)
                    ss << " [color=\"black\", penwidth=50.0];\n";
                else
                    ss << " [color=\"blue\", penwidth=10.0];\n";
            }
        }
    }
    ss << "}\n";

    std::string dot_content = ss.str();

    // Saving the string in a dot file

    std::filesystem::path filename_dot{filename_};

    std::ofstream dot_file(filename_dot.replace_extension(".dot"));
    if (!dot_file.is_open())
    {
        std::cerr << "Error creating file " << filename_dot.replace_extension(".dot") << std::endl;
        return;
    }
    dot_file << dot_content;
    dot_file.close();
}

void Graph::SwitchActiveConnection(uint64_t a, uint64_t b, bool activate)
{
    auto vertices = vertices_.data();
    auto i = a * vector_size_ + b;
    if (adj_[i] == State::INACTIVE && activate)
    {
        vertices[a]->AddActiveConnections();
        vertices[b]->AddActiveConnections();
    }
    if (adj_[i] == State::ACTIVE && !activate)
    {
        vertices[a]->DecActiveConnections();
        vertices[b]->DecActiveConnections();
    }
}

Graph::~Graph()
{
    for (const auto &vertex : vertices_)
        delete vertex;
    vertices_.clear();
    adj_.clear();
}
