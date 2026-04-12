#!/usr/bin/env python3
import sys
import json
import subprocess
import re
import statistics
from datetime import datetime
from pathlib import Path

# --- CONFIGURAÇÃO ---
RUNS = int(sys.argv[1]) if len(sys.argv) > 1 else 10
TIMEOUT_S = int(sys.argv[2]) if len(sys.argv) > 2 else 60

BASE_DIR = Path(__file__).resolve().parent.parent
BUILD_DIR = BASE_DIR / "build" / "bin"
if not (BUILD_DIR / "nemertea").exists() and not (BUILD_DIR / "nemertea.exe").exists():
    BUILD_DIR = BASE_DIR / "build" / "bin" / "Release"
GRAPH_DIR = BASE_DIR / "graphs"
RESULTS_DIR = BASE_DIR / "results"
TIMESTAMP = datetime.now().strftime("%Y%m%d_%H%M%S")

ALGOS = [
    {"name": "backtracking", "cmd": "{exe} {graph} {dot} {timeout}"},
    {"name": "warnsdorff", "cmd": "{exe} {graph} {dot}"},
    {"name": "nemertea", "cmd": "{exe} --graph {graph} --depth 7", "fixed_dot": True},
]


def parse_output(stdout):
    match_us = re.search(r"(\d+)\s+microseconds", stdout)
    if not match_us:
        match_us = re.search(r"Time:\s*(\d+)", stdout)

    us = int(match_us.group(1)) if match_us else -1

    status = "ERROR"
    if "=> SOLVED" in stdout or "FOUND" in stdout:
        status = "FOUND"
    elif "=> INCONCLUSIVE" in stdout:
        status = "INCONCLUSIVE"
    elif (
        "No Hamiltonian cycle" in stdout or "NOT FOUND" in stdout or "Found 0" in stdout
    ):
        status = "NOT_FOUND"
    elif "TIMEOUT" in stdout:
        status = "TIMEOUT"

    return us, status


def fmt_time(us):
    if us < 0:
        return "—"
    if us < 1000:
        return f"{us}µs"
    if us < 1000000:
        return f"{us/1000:.2f}ms"
    return f"{us/1000000:.3f}s"


def run_benchmark():
    RESULTS_DIR.mkdir(parents=True, exist_ok=True)
    csv_path = RESULTS_DIR / f"benchmark_{TIMESTAMP}.csv"

    graphs = sorted(list(GRAPH_DIR.glob("*.json")))

    with open(csv_path, "w") as csv_file:
        # Nova estrutura de colunas solicitada
        csv_file.write(
            "grafo,vertices,arestas,algoritmo,runs,found,status,mediana_us,min_us,max_us\n"
        )

        print(f"════════════════════════════════════════════════════════")
        print(f" Hamilton Benchmark — {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print(f" Runs: {RUNS} | Timeout: {TIMEOUT_S}s")
        print(f"════════════════════════════════════════════════════════\n")

        for graph_path in graphs:
            with open(graph_path) as f:
                g_data = json.load(f)
                v_count = len(g_data["vertex"])
                e_count = len(g_data["edge"])

            print(f"● {graph_path.stem} ({v_count}V, {e_count}E)")

            for algo in ALGOS:
                exe_path = BUILD_DIR / algo["name"]
                if not exe_path.exists():
                    exe_path = exe_path.with_suffix(".exe")
                if not exe_path.exists():
                    continue

                success_times = []
                failures_times = []
                found_count = 0
                any_found = False
                last_status = "NOT_FOUND"

                print(f"  → {algo['name']:14} | ", end="", flush=True)

                for i in range(RUNS):
                    dot_temp = RESULTS_DIR / f"temp_{algo['name']}.dot"
                    cmd = algo["cmd"].format(
                        exe=exe_path, graph=graph_path, dot=dot_temp, timeout=TIMEOUT_S
                    )

                    try:
                        proc = subprocess.run(
                            cmd.split(),
                            capture_output=True,
                            text=True,
                            timeout=TIMEOUT_S + 2,
                        )
                        us, status = parse_output(proc.stdout + proc.stderr)
                    except subprocess.TimeoutExpired:
                        us, status = -1, "TIMEOUT"

                    if status == "FOUND":
                        found_count += 1
                        any_found = True
                        if us >= 0:
                            success_times.append(us)
                    else:
                        failures_times.append(us)

                    last_status = status
                    print(f"{i+1} ", end="", flush=True)

                    if status in ["TIMEOUT", "ERROR"]:
                        break

                # Definição do status final: se encontrou em alguma vez, é FOUND
                final_status = "FOUND" if any_found else last_status

                # Estatísticas apenas sobre os acertos (FOUND)
                if success_times:
                    med = statistics.median(success_times)
                    v_min, v_max = min(success_times), max(success_times)
                else:
                    med = statistics.median(failures_times)
                    v_min, v_max = min(failures_times), max(failures_times)
                    # med = v_min = v_max = -1

                stat_str = f"| {final_status:12} | Found: {found_count}/{RUNS} | Mediana: {fmt_time(med)}"
                print(stat_str)

                csv_file.write(
                    f"{graph_path.stem},{v_count},{e_count},{algo['name']},{RUNS},"
                    f"{found_count},{final_status},{med},{v_min},{v_max}\n"
                )
            print("─" * 70)


if __name__ == "__main__":
    run_benchmark()
