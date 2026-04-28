# Nemertea: Territorial Expansion-Based Algorithm for the Hamiltonian Cycle Problem

Nemertea is a C++23 suite designed to solve and benchmark the **Hamiltonian Cycle Problem (HCP)**. Performance is also compared with classic approaches such as Backtracking and Warnsdorff's heuristic.

![Dodecahedron Graph with Hamiltonian Cycle](images/dodecahedron.png)

## 🧬 Algorithm Suite
This repository contains a unified environment to provide a comprehensive performance analysis:
- **Nemertea**: The proposed territorial expansion algorithm (High-performance BFS-based).
- **Backtracking**: A standard exhaustive search (Baseline).
- **Warnsdorff**: A greedy heuristic adapted for Hamiltonian paths.

## 🛠 Requirements

| Tool | Version | Notes |
| :--- | :--- | :--- |
| **CMake** | ≥ 3.21 | Build system generator |
| **C++ Compiler** | C++23 | GCC 13+, Clang 15+, or MSVC 2022 |
| **Python** | 3.8+ | Required for the **benchmark suite** |
| **Git** | any | To fetch `nlohmann/json` automatically |
| **samply** | any | Optional — for CPU profiling ([install](https://github.com/mstange/samply)) |

## 🚀 Build Instructions

The project uses a unified CMake structure. All executables are placed in `build/bin`.

### Linux / macOS
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

### Windows (PowerShell / CMD)
```powershell
cmake -B build
cmake --build build --config Release
```


## 📊 Benchmarking

We provide a specialized Python script to execute multiple runs, handle timeouts, and calculate performance metrics (Median, Min, Max) across different graphs.

### Running the Benchmark
From the root directory:
```bash
python3 scripts/benchmark.py <runs> <timeout_in_seconds>
```
*Example: `python3 scripts/benchmark.py 10 60`*

### Results
- **Terminal**: Real-time progress with status (`FOUND`, `NOT_FOUND`, `TIMEOUT`, `INCONCLUSIVE`).
- **CSV**: Detailed logs saved in `results/benchmark_TIMESTAMP.csv`.
- **DOT**: Generated cycles are saved in `results/dot_TIMESTAMP/` for visualization.

## 🔬 Profiling

CPU profiling via [samply](https://github.com/mstange/samply), which records stack samples and opens them in the [Firefox Profiler](https://profiler.firefox.com/) for interactive flame graphs and call trees. Works on **macOS, Linux, and Windows**.

### Prerequisites

Install **samply** from [github.com/mstange/samply](https://github.com/mstange/samply):

```bash
# macOS (Homebrew)
brew install samply

# Linux / macOS (via Cargo — requires Rust toolchain)
cargo install --locked samply

# Windows (PowerShell)
powershell -c "irm https://github.com/mstange/samply/releases/download/samply-v0.13.1/samply-installer.ps1 | iex"
```

On **Linux**, you may also need to allow unprivileged perf events:
```bash
echo 1 | sudo tee /proc/sys/kernel/perf_event_paranoid
```

### Running the Profiler

```bash
make profile <algo> <graph>
```

This builds with profiling flags, records a CPU profile, and opens it in Firefox.

**Examples:**
```bash
make profile nemertea frucht
make profile backtracking att48_tsp
make profile warnsdorff petersen
```

Press **Ctrl+C** to stop the symbol server when done.

### What to Look For
- **Call Tree**: See which functions consume the most CPU time
- **Flame Graph**: Visualize the full call stack depth and width

## 📂 Input & Data Format

The suite accepts graphs in JSON format (compatible with [Grafuria](https://codeberg.org/saulopz/grafuria)).

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

## 🧪 Manual Execution
If you wish to run a single test manually:
```bash
# Nemertea
./build/bin/nemertea --graph graphs/02_hypercube4d.json --depth 7

# Backtracking / Warnsdorff
./build/bin/backtracking graphs/02_hypercube4d.json output.dot 60
```

## Example Graphs

![RNET-58 Graph with Hamiltonian Cycle](images/rnet-58.png)


## 📈 Performance Results

The performance of **Nemertea** was evaluated against standard Backtracking and Warnsdorff's heuristic. The results below reflect the empirical data from `benchmark_20260401_132430.csv`.

### 🖥️ Execution Environment
To ensure scientific integrity and reproducibility, all benchmarks were executed in a controlled *sovereign computing environment*:
* **OS:** Kubuntu 25.10 (Questing Quokka) x86_64
* **Kernel:** Linux 6.17.0-20-generic
* **Processor:** 11th Gen Intel(R) Core(TM) i7-11800H (16) @ 4.60 GHz
* **Memory:** 30.62 GiB

### 📈 Key Results & Highlights (Comparative Analysis)
The Nemertea algorithm demonstrates superior efficiency, especially where traditional heuristics fail or reach computational limits:

| Graph Instance        | Vertices | Backtracking (med) | Warnsdorff (med) | **Nemertea (med)** |
| :---                  | :---:    | :---:              | :---:            | :---:              |
| **hoffman-singleton** | 50       | 3,882.0 $\mu s$    | 98.0 $\mu s$     | **14.0 $\mu s$**   |
| **watkins-snark**     | 50       | *TIMEOUT*          | 85.0 $\mu s$     | **4.0 $\mu s$**   |
| **graph1001_hcp**     | 9,528    | *TIMEOUT*          | 57,446.5 $\mu s$ | **85.5 $\mu s$**   |
| **att48_tsp**         | 48       | 156.0 $\mu s$      | 141.0 $\mu s$    | **22.0 $\mu s$**   |
| **berlin52_tsp**      | 52       | 323.0 $\mu s$      | 374.5 $\mu s$    | **25.0 $\mu s$**   |
| **rnet-58**           | 58       | *TIMEOUT*          | 219.0 $\mu s$    | **10.0 $\mu s$**   |

*Note: In `graph1001_hcp`, Warnsdorff failed to find a solution (`NOT_FOUND`), while Nemertea reached its state in a fraction of the time.*

#### Insights from the Study:
* **Scalability:** In the `graph1001_hcp` instance (9,528 vertices), Nemertea reached its state in only **85.5 $\mu s$**, while traditional Backtracking exceeded the 1-minute **TIMEOUT** threshold.
* **Practical Complexity:** Data confirms that Nemertea maintains a performance profile near $O(|V| + |E|)$ in sparse instances, avoiding the exponential growth typical of exhaustive searches.
* **Efficiency in Density:** For dense graphs (TSPLIB), Nemertea is consistently **7x to 12x faster** than the baseline. Even considering its probabilistic nature, the cumulative time for multiple runs remains orders of magnitude lower than a single backtracking session.


## 📜 License & Scientific Attribution

This project is licensed under the MIT License. See the LICENSE file for details.

Scientific Integrity & Intellectual Property:

- Algorithm Registration: The Nemertea algorithm and its implementation are officially registered with the Brazilian National Institute of Industrial Property (INPI) under registration numbers BR512025005332-0 (v1.0) and BR512026002289-4 (v1.1).
- Peer-Review Status: This repository provides the source code and benchmarking suite associated with a manuscript currently under review at IEEE Latin America Transactions.
- Citation: If you use this software for research purposes, please cite the associated work (citation details will be updated upon final publication).

**Contact:** **Saulo Popov Zambiasi** (saulopz@gmail.com)

Professor and Researcher

## Acknowledgments (GitHub Version)

I would like to thank **[Nassor Frazier-Silva](https://github.com/nassor)** for his invaluable contribution to this project, specifically for:

- Refining the `CMakeLists.txt` for macOS compatibility.
- Implementing the profiling scripts for performance and CPU consumption analysis, along with the corresponding tutorial in the `README.md`.

This collaboration was essential for ensuring the technical robustness of the implementation across different environments.