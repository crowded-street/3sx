#!/usr/bin/env python3
"""Sweep every first-party source file through CodeScene Code Health.

Drives the CodeScene MCP server (cs-mcp) over stdio and records a Code Health
score per file, so campaign progress can be compared against a committed
baseline. See docs/refactoring/README.md.

The server is asked for scores in small chunks. Sending all files in one
session makes the CodeScene CLI die with java.lang.OutOfMemoryError, because
the server answers requests concurrently and spawns a CLI process per request.

Requires CS_ACCESS_TOKEN in the environment.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent

# Vendored or generated; not ours to refactor.
EXCLUDE_PREFIXES = (
    "src/imgui/",
    "src/stb/",
    "src/argparse/",
    "src/bin2obj/",
)

SCORE_RE = re.compile(r"Code Health score:\s*([0-9.]+)")
CHUNK_DEFAULT = 16


def find_binary() -> str:
    """Locate the cs-mcp binary, preferring an explicit override."""
    override = os.environ.get("CS_MCP_BINARY_PATH")
    if override:
        if not Path(override).is_file():
            sys.exit("CS_MCP_BINARY_PATH is set but does not exist: " + override)
        return override

    on_path = shutil.which("cs-mcp")
    if on_path:
        return on_path

    try:
        npm_root = subprocess.run(
            ["npm", "root", "-g"], capture_output=True, text=True, shell=(os.name == "nt")
        ).stdout.strip()
    except OSError:
        npm_root = ""

    if npm_root:
        pkg = Path(npm_root) / "@codescene" / "codehealth-mcp"
        cache = pkg / ".cache"
        if cache.is_dir():
            for version_dir in sorted(cache.iterdir(), reverse=True):
                for candidate in ("cs-mcp.exe", "cs-mcp"):
                    exe = version_dir / candidate
                    if exe.is_file():
                        return str(exe)

    sys.exit(
        "Could not find cs-mcp. Install it with:\n"
        "  npm install -g @codescene/codehealth-mcp\n"
        "then run it once, or set CS_MCP_BINARY_PATH."
    )


def candidates() -> list[str]:
    out = subprocess.run(
        ["git", "ls-files"], cwd=REPO, capture_output=True, text=True
    ).stdout
    files = []
    for line in out.splitlines():
        path = line.strip()
        if not path.endswith((".c", ".cpp")):
            continue
        if any(path.startswith(p) for p in EXCLUDE_PREFIXES):
            continue
        files.append(path)
    return sorted(files)


def run_chunk(exe: str, paths: list[str], first_id: int) -> tuple[dict[int, str], dict[int, str]]:
    """Score one chunk. Returns ({request id: response text}, {request id: path})."""
    lines = [
        json.dumps(
            {
                "jsonrpc": "2.0",
                "id": 1,
                "method": "initialize",
                "params": {
                    "protocolVersion": "2024-11-05",
                    "capabilities": {},
                    "clientInfo": {"name": "codehealth-sweep", "version": "1.0.0"},
                },
            }
        ),
        json.dumps({"jsonrpc": "2.0", "method": "notifications/initialized"}),
    ]
    ids = {}
    for offset, rel in enumerate(paths):
        rid = first_id + offset
        ids[rid] = rel
        lines.append(
            json.dumps(
                {
                    "jsonrpc": "2.0",
                    "id": rid,
                    "method": "tools/call",
                    "params": {
                        "name": "code_health_score",
                        # Forward slashes: backslashes get mangled through several layers.
                        "arguments": {"file_path": str(REPO).replace(os.sep, "/") + "/" + rel},
                    },
                }
            )
        )
    stdin = "\n".join(lines) + "\n"

    env = dict(os.environ, CS_DISABLE_VERSION_CHECK="1")
    proc = subprocess.run(
        [exe], input=stdin, capture_output=True, text=True, env=env, encoding="utf-8", errors="replace"
    )
    if "OutOfMemoryError" in (proc.stderr or ""):
        sys.exit("CodeScene CLI ran out of memory. Retry with a smaller --chunk.")

    out = {}
    for line in (proc.stdout or "").splitlines():
        line = line.strip()
        if not line:
            continue
        try:
            msg = json.loads(line)
        except json.JSONDecodeError:
            continue
        rid = msg.get("id")
        if rid not in ids:
            continue
        result = msg.get("result", {})
        text = " ".join(c.get("text", "") for c in result.get("content", []))
        out[rid] = "" if (result.get("isError") or "error" in msg) else text
    return {rid: out.get(rid, "") for rid in ids}, ids


def band(score: float) -> str:
    if score >= 10.0:
        return "optimal"
    if score >= 9.0:
        return "green"
    if score >= 4.0:
        return "yellow"
    return "red"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("-o", "--output", default=str(REPO / "codehealth-baseline.json"))
    ap.add_argument("--chunk", type=int, default=CHUNK_DEFAULT,
                    help="files per server invocation (default %(default)s)")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    if not os.environ.get("CS_ACCESS_TOKEN"):
        print("warning: CS_ACCESS_TOKEN is not set; standalone analysis only", file=sys.stderr)

    exe = find_binary()
    files = candidates()
    if not args.quiet:
        print("cs-mcp:  " + exe)
        print("scoring: " + str(len(files)) + " files in chunks of " + str(args.chunk))

    scores, unscorable = {}, []
    for start in range(0, len(files), args.chunk):
        chunk = files[start:start + args.chunk]
        responses, ids = run_chunk(exe, chunk, 100 + start)
        for rid, text in responses.items():
            rel = ids[rid]
            match = SCORE_RE.search(text or "")
            if match:
                scores[rel] = float(match.group(1))
            else:
                unscorable.append(rel)
        if not args.quiet:
            done = min(start + args.chunk, len(files))
            print("  " + str(done) + "/" + str(len(files)), end="\r", flush=True)

    if not args.quiet:
        print(" " * 40, end="\r")

    counts = {"red": 0, "yellow": 0, "green": 0, "optimal": 0}
    for value in scores.values():
        counts[band(value)] += 1

    payload = {
        "total_candidates": len(files),
        "scored": len(scores),
        "unscorable": len(unscorable),
        "bands": counts,
        "scores": [{"path": p, "score": s} for p, s in sorted(scores.items(), key=lambda kv: kv[1])],
        "unscorable_files": sorted(unscorable),
    }
    Path(args.output).write_text(json.dumps(payload, indent=1) + "\n", encoding="utf-8")

    print("scored " + str(len(scores)) + " files (" + str(len(unscorable)) + " unscorable - data tables)")
    total = max(len(scores), 1)
    for name in ("red", "yellow", "green", "optimal"):
        n = counts[name]
        print("  " + name.ljust(8) + str(n).rjust(4) + "  " + format(100.0 * n / total, "5.1f") + "%")
    print("wrote " + args.output)

    worst = payload["scores"][:5]
    if worst:
        print("worst files:")
        for row in worst:
            print("  " + format(row["score"], "5.2f") + "  " + row["path"])
    return 0


if __name__ == "__main__":
    sys.exit(main())
