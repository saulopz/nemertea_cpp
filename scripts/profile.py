#!/usr/bin/env python3
"""Profile a single algorithm on a single graph using samply.

Opens the profile in Firefox Profiler with live symbol resolution.

Usage:
    python3 scripts/profile.py <algo> <graph> [--iters N] [--timeout S] [--no-view]

Examples:
    python3 scripts/profile.py nemertea frucht
    python3 scripts/profile.py backtracking att48_tsp --iters 50000
    python3 scripts/profile.py warnsdorff petersen --no-view

Requirements:
    samply — https://github.com/mstange/samply
    Works on macOS, Linux, and Windows.
"""

import argparse
import json
import os
import platform
import re
import shutil
import subprocess
import sys
import time
import urllib.request
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent.parent
BIN_DIR = BASE_DIR / "build" / "bin"
GRAPH_DIR = BASE_DIR / "graphs"
PROFILE_DIR = BASE_DIR / "results" / "profiles"

EXE_SUFFIX = ".exe" if platform.system() == "Windows" else ""


def _find_firefox() -> str | None:
    """Return the path to Firefox, or None if not found."""
    system = platform.system()
    if system == "Darwin":
        app = "/Applications/Firefox.app/Contents/MacOS/firefox"
        if os.path.isfile(app):
            return app
    elif system == "Windows":
        for base in [os.environ.get("PROGRAMFILES", ""), os.environ.get("PROGRAMFILES(X86)", "")]:
            if base:
                candidate = os.path.join(base, "Mozilla Firefox", "firefox.exe")
                if os.path.isfile(candidate):
                    return candidate
    # Linux / fallback: check PATH
    path = shutil.which("firefox")
    return path


def _get_profiler_ui_url(port: int, retries: int = 10) -> str | None:
    """Wait for the samply symbol server and return the profiler UI URL."""
    url = f"http://127.0.0.1:{port}/"
    for _ in range(retries):
        time.sleep(0.5)
        try:
            with urllib.request.urlopen(url, timeout=2) as resp:
                html = resp.read().decode()
            # Extract the "Open the profile in the profiler UI" href
            m = re.search(r'href="(https://profiler\.firefox\.com/[^"]+)"', html)
            if m:
                return m.group(1)
        except (urllib.error.URLError, OSError):
            continue
    return None


def _open_in_firefox(firefox_path: str, url: str):
    """Open a URL in Firefox (cross-platform)."""
    if platform.system() == "Darwin":
        subprocess.Popen(["open", "-a", "Firefox", url])
    else:
        subprocess.Popen([firefox_path, url])

ALGOS = {
    "backtracking": lambda exe, graph, timeout, iters: [
        str(exe), str(graph), os.devnull, str(timeout),
        "--profile-iters", str(iters)
    ],
    "warnsdorff": lambda exe, graph, timeout, iters: [
        str(exe), str(graph), os.devnull,
        "--profile-iters", str(iters)
    ],
    "nemertea": lambda exe, graph, timeout, iters: [
        str(exe), "--graph", str(graph), "--depth", "12",
        "--profile-iters", str(iters)
    ],
}


def check_samply():
    if shutil.which("samply") is None:
        print("Error: 'samply' is not installed.", file=sys.stderr)
        print("See https://github.com/mstange/samply for installation:", file=sys.stderr)
        print("  macOS:    brew install samply", file=sys.stderr)
        print("  Linux:    cargo install --locked samply", file=sys.stderr)
        print("  Windows:  powershell -c \"irm https://github.com/mstange/samply/"
              "releases/download/samply-v0.13.1/samply-installer.ps1 | iex\"",
              file=sys.stderr)
        sys.exit(1)


def graph_info(path: Path) -> tuple[str, int, int]:
    """Return (stem, vertex_count, edge_count) for a graph JSON file."""
    with open(path) as f:
        data = json.load(f)
    return path.stem, len(data.get("vertex", [])), len(data.get("edge", []))


def profile_save(algo: str, exe: Path, graph: Path, timeout: int,
                 iters: int, rate: int, out_dir: Path) -> Path | None:
    """Record + save profile to disk (--no-view mode).

    Uses --unstable-presymbolicate so that saved profiles include symbol
    data for later viewing with ``samply load``.
    """
    stem, v, e = graph_info(graph)
    profile_path = out_dir / f"{stem}.json"
    cmd_args = ALGOS[algo](exe, graph, timeout, iters)

    record_cmd = [
        "samply", "record", "--save-only", "--unstable-presymbolicate",
        "-r", str(rate), "-o", str(profile_path), "--",
    ] + cmd_args

    print(f"  {algo}/{stem} ({v}V, {e}E) ...", end=" ", flush=True)

    try:
        result = subprocess.run(
            record_cmd,
            timeout=timeout + 30,
            capture_output=True,
            text=True,
        )
        if result.returncode == 0 and profile_path.exists():
            size = profile_path.stat().st_size
            print(f"OK ({size // 1024} KB)")
            return profile_path
        else:
            print("FAILED")
            if result.stderr.strip():
                for line in result.stderr.strip().splitlines()[:3]:
                    print(f"    {line}")
            return None
    except subprocess.TimeoutExpired:
        print("TIMEOUT")
        return None


def profile_view(algo: str, exe: Path, graph: Path, timeout: int,
                 iters: int, rate: int, firefox_path: str, port: int = 3000):
    """Record a profile and open it in Firefox with live symbol resolution.

    Uses ``samply record --no-open`` which keeps the binary context in
    memory so the symbol server can resolve addresses to function names.
    Blocks until Ctrl+C so the symbol server stays alive.
    """
    stem, v, e = graph_info(graph)
    cmd_args = ALGOS[algo](exe, graph, timeout, iters)

    record_cmd = [
        "samply", "record", "--no-open", "-r", str(rate),
        "--port", str(port), "--",
    ] + cmd_args

    print(f"  {algo}/{stem} ({v}V, {e}E) (port {port}) ...", end=" ", flush=True)

    proc = subprocess.Popen(
        record_cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )
    profiler_url = _get_profiler_ui_url(port)
    if profiler_url:
        _open_in_firefox(firefox_path, profiler_url)
        print("OK")
        print()
        print("Symbol server running. Press Ctrl+C to stop.")
        try:
            proc.wait()
        except KeyboardInterrupt:
            proc.terminate()
    else:
        print("FAILED (symbol server did not start)")
        if proc.poll() is not None:
            out = proc.stdout.read() if proc.stdout else ""
            if out.strip():
                for line in out.strip().splitlines()[:3]:
                    print(f"    {line}")
        sys.exit(1)


def _resolve_graph(name: str) -> Path:
    """Find a graph JSON file by name or substring."""
    graphs = sorted(GRAPH_DIR.glob("*.json"))
    if not graphs:
        print(f"Error: No .json graph files in {GRAPH_DIR}", file=sys.stderr)
        sys.exit(1)

    needle = name.replace(".json", "")
    matches = [g for g in graphs if needle in g.stem]
    if not matches:
        print(f"Error: No graph matching '{name}'", file=sys.stderr)
        print(f"Available: {', '.join(g.stem for g in graphs)}", file=sys.stderr)
        sys.exit(1)
    if len(matches) > 1:
        print(f"Error: '{name}' matches multiple graphs:", file=sys.stderr)
        for m in matches:
            print(f"  {m.stem}", file=sys.stderr)
        sys.exit(1)
    return matches[0]


def main():
    parser = argparse.ArgumentParser(
        description="Profile a single algorithm on a single graph using samply.")
    parser.add_argument("algo", choices=list(ALGOS.keys()),
                        help="Algorithm to profile")
    parser.add_argument("graph", type=str,
                        help="Graph to profile (filename or substring, e.g. 'frucht')")
    parser.add_argument("--iters", type=int, default=50000,
                        help="Algorithm iterations for profiling (default: 50000)")
    parser.add_argument("--rate", type=int, default=50000,
                        help="Sampling rate in Hz (default: 50000)")
    parser.add_argument("--timeout", type=int, default=60,
                        help="Backtracking timeout in seconds (default: 60)")
    parser.add_argument("--no-view", action="store_true",
                        help="Save profile to disk instead of opening in Firefox")
    args = parser.parse_args()

    check_samply()

    graph = _resolve_graph(args.graph)
    exe = BIN_DIR / f"{args.algo}{EXE_SUFFIX}"
    if not exe.exists():
        print(f"Error: binary not found at {exe}", file=sys.stderr)
        print("Run 'make profile-build' first.", file=sys.stderr)
        sys.exit(1)

    print(f"=== Profiling {args.algo} on {graph.stem} ===")
    print(f"  Iterations:  {args.iters}")
    print(f"  Sample rate: {args.rate} Hz")
    print(f"  Timeout:     {args.timeout}s")
    print()

    if args.no_view:
        out_dir = PROFILE_DIR / args.algo
        out_dir.mkdir(parents=True, exist_ok=True)
        path = profile_save(args.algo, exe, graph, args.timeout,
                            args.iters, args.rate, out_dir)
        if path:
            print()
            print(f"Saved: {path}")
            print(f"View with: samply load {path}")
    else:
        firefox_path = _find_firefox()
        if firefox_path is None:
            print("Error: Firefox not found. Install Firefox or use --no-view.",
                  file=sys.stderr)
            sys.exit(1)
        profile_view(args.algo, exe, graph, args.timeout,
                     args.iters, args.rate, firefox_path)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nStopping.")
        sys.exit(0)