#!/usr/bin/env python3
"""Score or review files with CodeScene, locally and in one batch.

tools/codehealth_sweep.py drives the whole repository and writes a baseline.
This is the same transport aimed at the inner loop instead: hand it the files
you just changed and get their scores, or their findings, back in one server
invocation.

    python tools/ch.py src/.../a.c src/.../b.c          # scores
    python tools/ch.py --review src/.../a.c             # findings, one per line
    python tools/ch.py --json   src/.../a.c             # the raw review

Findings print as "category (indication) n functions", which is what rule 2
asks for: the score alone cannot say whether a targeted function left a smell.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
EXE = os.environ.get("CS_MCP_BINARY_PATH", "/opt/homebrew/bin/cs-mcp")
SCORE_RE = re.compile(r"Code Health score:\s*([0-9.]+)")


def call(tool: str, paths: list[str], chunk: int = 12) -> dict[str, str]:
    """{path: response text} for one tool over every path, chunked."""
    out: dict[str, str] = {}
    for start in range(0, len(paths), chunk):
        part = paths[start:start + chunk]
        lines = [
            json.dumps({"jsonrpc": "2.0", "id": 1, "method": "initialize",
                        "params": {"protocolVersion": "2024-11-05", "capabilities": {},
                                   "clientInfo": {"name": "ch", "version": "1.0.0"}}}),
            json.dumps({"jsonrpc": "2.0", "method": "notifications/initialized"}),
        ]
        ids = {}
        for offset, rel in enumerate(part):
            rid = 100 + start + offset
            ids[rid] = rel
            absolute = str((REPO / rel).resolve()).replace(os.sep, "/")
            lines.append(json.dumps({"jsonrpc": "2.0", "id": rid, "method": "tools/call",
                                     "params": {"name": tool,
                                                "arguments": {"file_path": absolute}}}))
        proc = subprocess.run([EXE], input="\n".join(lines) + "\n", capture_output=True,
                              text=True, encoding="utf-8", errors="replace",
                              env=dict(os.environ, CS_DISABLE_VERSION_CHECK="1"))
        if "OutOfMemoryError" in (proc.stderr or ""):
            sys.exit("CodeScene CLI ran out of memory. Retry with a smaller --chunk.")
        for line in (proc.stdout or "").splitlines():
            try:
                msg = json.loads(line.strip())
            except (json.JSONDecodeError, ValueError):
                continue
            if msg.get("id") not in ids:
                continue
            result = msg.get("result", {})
            text = " ".join(c.get("text", "") for c in result.get("content", []))
            out[ids[msg["id"]]] = "" if (result.get("isError") or "error" in msg) else text
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="+")
    ap.add_argument("--review", action="store_true", help="findings, not just the score")
    ap.add_argument("--json", action="store_true", help="the raw review, unformatted")
    ap.add_argument("--chunk", type=int, default=12)
    args = ap.parse_args()

    rels = []
    for f in args.files:
        p = Path(f).resolve()
        rels.append(str(p.relative_to(REPO)) if p.is_relative_to(REPO) else str(p))

    tool = "code_health_review" if (args.review or args.json) else "code_health_score"
    answers = call(tool, rels, args.chunk)

    total = 0.0
    for rel in rels:
        text = answers.get(rel, "")
        if tool == "code_health_score":
            m = SCORE_RE.search(text)
            print("%5s  %s" % (m.group(1) if m else "-", rel))
            total += float(m.group(1)) if m else 0.0
            continue
        try:
            data = json.loads(text)
        except (json.JSONDecodeError, ValueError):
            print("%s: unscorable" % rel)
            continue
        if args.json:
            print(json.dumps({rel: data}, indent=1))
            continue
        print("%5s  %s" % (data.get("score", "-"), rel))
        for r in data.get("review", []):
            n = len(r.get("functions", []) or [])
            print("         %-34s indication %s%s"
                  % (r["category"], r.get("indication", "-"),
                     ", %d functions" % n if n else ""))
    if tool == "code_health_score" and len(rels) > 1:
        print("%5.2f  mean of %d" % (total / len(rels), len(rels)))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
