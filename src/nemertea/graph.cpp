// -*- coding: utf-8 -*-
// ============================================================================
// Project: Nemertea
// File: graph.cpp
//
// Part of the Nemertea Project
// Territorial Expansion-Based Algorithm for the Hamiltonian Cycle Problem
//
// © 2021-Present Saulo Popov Zambiasi. All rights reserved.
// Registered at INPI [BR512025005332-0].
//
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.
// ============================================================================
// Description:
// Implementation of the Graph class, which represents a graph structure consisting
// of vertices and edges, and provides methods for loading, saving, and managing
// the graph. The Graph class uses an adjacency matrix to track the state of
// connections between vertices, allowing for efficient updates and queries of
// connection states. The class also includes functionality for random vertex
// selection, which can be useful for various graph algorithms and operations.
// ============================================================================

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
                SetEdgeState(a_id, b_id, State::INACTIVE);
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

        if (vertex->GetState() == State::CONQUERED)
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
                if (adj_[i * n + j] == State::CONQUERED)
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

Graph::~Graph()
{
    for (const auto &vertex : vertices_)
        delete vertex;
    vertices_.clear();
    adj_.clear();
}
