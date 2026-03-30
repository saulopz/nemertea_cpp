#include "graph.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <algorithm>
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
//  load_graph
// ─────────────────────────────────────────────────────────────────────────────

Graph load_graph(const std::string& path)
{
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Cannot open file: " + path);

    nlohmann::json j;
    f >> j;

    Graph g;
    g.bidirectional = j.value("bidirectional", false);

    for (auto& v : j["vertex"]) {
        Vertex vtx;
        vtx.id   = v["id"].get<int>();
        vtx.name = v.value("name", "");
        vtx.x    = v["x"].get<double>();
        vtx.y    = v["y"].get<double>();
        g.id_to_idx[vtx.id] = static_cast<int>(g.vertices.size());
        g.vertices.push_back(vtx);

        g.adj[vtx.id] = {};
    }

    for (auto& e : j["edge"]) {
        Edge edge;
        edge.id     = e["id"].get<int>();
        edge.a      = e["a"].get<int>();
        edge.b      = e["b"].get<int>();
        edge.weight = e.value("weight", 1.0);
        g.edges.push_back(edge);

        g.adj[edge.a].push_back(edge.b);
        g.adj[edge.b].push_back(edge.a);


        //g.adj[edge.a].push_back(edge.b);
        //if (g.bidirectional)
        //    g.adj[edge.b].push_back(edge.a);
    }

    return g;
}

// ─────────────────────────────────────────────────────────────────────────────
//  write_dot
// ─────────────────────────────────────────────────────────────────────────────

void write_dot(const std::string&      out_path,
               const std::string&      in_path,
               const std::string&      algo_name,
               const Graph&            g,
               long long               elapsed_us,
               const std::vector<int>& hc_path_ids)
{
    // ── Build lookup sets for fast O(1) membership tests ─────────────────────
    std::unordered_set<int> hc_verts;

    // Edge key: pack two ints into one long long (order-independent)
    auto edge_key = [](int a, int b) -> long long {
        if (a > b) std::swap(a, b);
        return static_cast<long long>(a) * 1'000'000LL + b;
    };

    std::unordered_set<long long> hc_edges;

    if (!hc_path_ids.empty()) {
        for (int id : hc_path_ids)
            hc_verts.insert(id);

        for (size_t k = 0; k + 1 < hc_path_ids.size(); ++k)
            hc_edges.insert(edge_key(hc_path_ids[k], hc_path_ids[k + 1]));
    }

    // ── Open output ──────────────────────────────────────────────────────────
    std::ofstream out(out_path);
    if (!out.is_open())
        throw std::runtime_error("Cannot write: " + out_path);

    // ── Header comment ───────────────────────────────────────────────────────
    out << "// Graph name: " << in_path << "\n";
    out << "// Algorithm : " << algo_name << "\n";

    if (elapsed_us < 0)
        out << "// Run time  : TIMEOUT\n";
    else
        out << "// Run time  : " << elapsed_us << " us\n";

    {
        std::time_t t = std::time(nullptr);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        out << "// Date      : " << buf << "\n";
    }

    if (hc_path_ids.empty())
        out << "// Result    : No Hamiltonian cycle found\n";
    else
        out << "// Result    : Hamiltonian cycle found (" << hc_verts.size() << " vertices)\n";

    // ── DOT body ─────────────────────────────────────────────────────────────
    out << "graph G {\n"
        << "\tlayout=neato;\n"
        << "\toverlap=false;\n"
        << "\tsplines=true;\n"
        << "\tnode [pin=true];\n"
        << "\tnode [shape=circle, width=1.0, height=1.0, "
           "fixedsize=true, style=filled, fillcolor=\"lightblue\"];\n\n";

    // Vertices
    for (auto& v : g.vertices) {
        bool in_hc = hc_verts.count(v.id) > 0;
        out << "\tV" << v.id
            << " [label=\"\", style=filled"
            << ", pos=\"" << v.x / 10.0 << "," << v.y / 10.0 << "!\""
            << ", fillcolor=\"" << (in_hc ? "black" : "lightblue") << "\""
            << ", color=\"black\"];\n";
    }
    out << "\n";

    // Edges
    for (auto& e : g.edges) {
        bool in_hc = hc_edges.count(edge_key(e.a, e.b)) > 0;
        out << "\tV" << e.a << " -- V" << e.b
            << " [color=\""   << (in_hc ? "black" : "blue")   << "\""
            << ", penwidth="  << (in_hc ? 50.0    : 10.0)     << "];\n";
    }

    out << "}\n";
}
