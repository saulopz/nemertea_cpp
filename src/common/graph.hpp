#include <string>
#include <unordered_map>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
//  Data structures
// ─────────────────────────────────────────────────────────────────────────────

struct Vertex
{
    int id;
    std::string name;
    double x, y;
};

struct Edge
{
    int id, a, b;
    double weight;
};

struct Graph
{
    bool bidirectional = false;
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;

    // vertex id  →  index in vertices[]
    std::unordered_map<int, int> id_to_idx;

    // vertex id  →  list of neighbour ids  (raw adjacency, not index-based)
    std::unordered_map<int, std::vector<int>> adj;
};

// ─────────────────────────────────────────────────────────────────────────────
//  I/O
// ─────────────────────────────────────────────────────────────────────────────

// Load graph from a JSON file (uses nlohmann/json via FetchContent).
// Throws std::runtime_error on failure.
Graph load_graph(const std::string &path);

// Write a Graphviz DOT file.
//   algo_name   – shown in the header comment (e.g. "Backtracking")
//   in_path     – original JSON path, shown in the header comment
//   elapsed_us  – algorithm-only time in microseconds (-1 = timed out)
//   hc_path_ids – vertex ids forming the Hamiltonian cycle (empty = not found)
//                 First and last element must be equal (closed cycle).
void write_dot(const std::string &out_path, const std::string &in_path, const std::string &algo_name, const Graph &g,
               long long elapsed_us, const std::vector<int> &hc_path_ids);
