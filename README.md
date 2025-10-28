# Nemertea: Territorial Expansion-Based Algorithm for the Hamiltonian Cycle Problem

Nemertea is a C++ implementation of a territorial-expansion algorithm to tackle the Hamiltonian Cycle Problem (HCP).

![Dodecahedron Graph with Hamiltonian Cycle](images/dodecahedron.png)

# Images

- [images/dodecahedron.png](images/dodecahedron.png): Dodecahedron graph with Hamiltonian cycle highlighted.

## Features
- Territorial expansion strategy to find Hamiltonian cycles.
- Command-line interface for execution and configuration.
- Accepts input graphs in JSON format.

## Requirements
- C++ compiler with C++23 support (g++ >= 11 recommended)
- make
- nlohmann/json (header-only)
  - Install on Debian/Ubuntu: `sudo apt install nlohmann-json3-dev`
  - Or download `single_include/nlohmann` and place it in `src/` (Makefile uses `-Isrc`)

## Input JSON format
Example (valid JSON — no trailing commas):

```json
{
  "bidirectional": true,
  "vertex": [
    { "name": "A", "id": 0, "x": 0, "y": 0 },
    { "name": "B", "id": 1, "x": 10, "y": 10 }
  ],
  "edge": [
    { "id": 0, "a": 0, "b": 1, "weight": 1.0 }
  ]
}
```

Fields:
- `bidirectional`: whether the graph is undirected.
- `vertex`: list of vertices with `name`, `id`, `x`, `y`.
- `edge`: list of edges with `id`, `a`, `b`, `weight` (`a` and `b` are vertex ids).

## Build
From project root:

```bash
make
```

Executable: `bin/nemertea`

To enable debug symbols (helpful for segfaults), edit `Makefile` and replace `-O2` with `-g` in `CXXFLAGS`.

## Run
Example:

```bash
bin/nemertea --graph graphs/my_graph.json --depth 7
```

Adjust parameters as needed.

# Graph Results

- The algotithm outputs whether a Hamiltonian cycle was found and the cycle itself if successful in dot format (graphviz) with same name as input file but with `.dot` extension.

## Example Graphs

![Big Random Graph with Hamiltonian Cycle](images/big_random.png)

## Debug tips
- If you get "no input files", ensure `src/` contains `.cpp` files.
- If compilation fails due to `nlohmann/json.hpp`, either install the system package or place the full `single_include/nlohmann` directory under `src/`.
- Avoid shared_ptr cycles: use `weak_ptr` for back-references (e.g., Edge -> Vertex).
- Use `std::enable_shared_from_this<T>` and `std::make_shared<T>()` when calling `shared_from_this()`.
- Always check pointers returned by functions (`nullptr`) before dereferencing.

## License & Contact
© 2025 Saulo Popov Zambiasi — Contact: saulopz@gmail.com

