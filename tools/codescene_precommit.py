#!/usr/bin/env python3
"""Block commits when staged code health degrades or new code is not perfect.

Policy:
- staged files that already existed in HEAD must not score lower than their HEAD score
- newly added source files must score exactly 10.0
- generated or vendored files are ignored

This is intentionally lightweight and follows the same CodeScene MCP path already used by
`tools/codehealth_sweep.py`.
"""

from __future__ import annotations

import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
EXCLUDE_PREFIXES = (
    "src/imgui/",
    "src/stb/",
    "src/argparse/",
    "src/bin2obj/",
)
SCORE_RE = re.compile(r"Code Health score:\s*([0-9.]+)")
CODE_EXTENSIONS = (".c", ".cpp", ".h", ".hpp")


def run(cmd: list[str], cwd: Path = REPO) -> subprocess.CompletedProcess[str]:
    return subprocess.run(cmd, cwd=str(cwd), capture_output=True, text=True, encoding="utf-8", errors="replace")


def find_cs_mcp() -> str:
    override = os.environ.get("CS_MCP_BINARY_PATH")
    if override:
        if not Path(override).is_file():
            sys.exit(
                "CodeScene CLI not found at CS_MCP_BINARY_PATH=\"" + override + "\".\n"
                "Install it with:\n"
                "  npm install -g @codescene/codehealth-mcp\n"
                "then run it once, or set CS_MCP_BINARY_PATH to the executable location."
            )
        return override

    on_path = shutil.which("cs-mcp")
    if on_path:
        return on_path

    sys.exit(
        "CodeScene CLI is not installed or not on PATH.\n"
        "Install it with:\n"
        "  npm install -g @codescene/codehealth-mcp\n"
        "Then verify it works with:\n"
        "  cs-mcp --help\n"
        "or set CS_MCP_BINARY_PATH to the installed executable."
    )


def staged_files() -> list[str]:
    result = run(["git", "diff", "--cached", "--name-only", "--diff-filter=ACMR"])
    if result.returncode != 0:
        sys.exit(result.stderr.strip() or "git diff failed")

    files: list[str] = []
    for line in result.stdout.splitlines():
        path = line.strip()
        if not path:
            continue
        if not path.endswith(CODE_EXTENSIONS):
            continue
        if any(path.startswith(prefix) for prefix in EXCLUDE_PREFIXES):
            continue
        files.append(path)
    return sorted(files)


def git_show(ref: str, path: str) -> str:
    result = run(["git", "show", f"{ref}:{path}"])
    if result.returncode != 0:
        return ""
    return result.stdout


def score_path(exe: str, file_path: str) -> float:
    payload = [
        json.dumps({
            "jsonrpc": "2.0",
            "id": 1,
            "method": "initialize",
            "params": {
                "protocolVersion": "2024-11-05",
                "capabilities": {},
                "clientInfo": {"name": "codescene-precommit", "version": "1.0.0"},
            },
        }),
        json.dumps({"jsonrpc": "2.0", "method": "notifications/initialized"}),
        json.dumps({
            "jsonrpc": "2.0",
            "id": 2,
            "method": "tools/call",
            "params": {
                "name": "code_health_score",
                "arguments": {"file_path": file_path},
            },
        }),
    ]
    stdin = "\n".join(payload) + "\n"
    env = dict(os.environ, CS_DISABLE_VERSION_CHECK="1")
    proc = subprocess.run(
        [exe],
        input=stdin,
        capture_output=True,
        text=True,
        env=env,
        encoding="utf-8",
        errors="replace",
    )
    if proc.returncode != 0 and not proc.stdout:
        raise RuntimeError((proc.stderr or proc.stdout or "CodeScene invocation failed").strip())

    for line in (proc.stdout or "").splitlines():
        line = line.strip()
        if not line:
            continue
        try:
            msg = json.loads(line)
        except json.JSONDecodeError:
            continue
        if msg.get("id") != 2:
            continue
        result = msg.get("result", {})
        text = " ".join(part.get("text", "") for part in result.get("content", []))
        match = SCORE_RE.search(text)
        if match:
            return float(match.group(1))
    raise RuntimeError("No Code Health score returned for: " + file_path)


def main() -> int:
    staged = staged_files()
    if not staged:
        return 0

    exe = find_cs_mcp()
    failures: list[str] = []

    for rel in staged:
        try:
            head_content = git_show("HEAD", rel)
            index_content = git_show(":" + rel, rel)
            if not index_content:
                continue

            if head_content:
                with tempfile.NamedTemporaryFile("w", suffix=Path(rel).name, delete=False, encoding="utf-8") as head_file:
                    head_file.write(head_content)
                    head_path = head_file.name
                with tempfile.NamedTemporaryFile("w", suffix=Path(rel).name, delete=False, encoding="utf-8") as staged_file:
                    staged_file.write(index_content)
                    staged_path = staged_file.name

                try:
                    original_score = score_path(exe, head_path)
                    new_score = score_path(exe, staged_path)
                    if new_score < original_score - 1e-9:
                        failures.append(
                            f"{rel}: Code Health degraded from {original_score:.2f} to {new_score:.2f}"
                        )
                finally:
                    os.unlink(head_path)
                    os.unlink(staged_path)
            else:
                with tempfile.NamedTemporaryFile("w", suffix=Path(rel).name, delete=False, encoding="utf-8") as staged_file:
                    staged_file.write(index_content)
                    staged_path = staged_file.name
                try:
                    new_score = score_path(exe, staged_path)
                    if abs(new_score - 10.0) > 1e-9:
                        failures.append(f"{rel}: new code must score 10.0, got {new_score:.2f}")
                finally:
                    os.unlink(staged_path)
        except Exception as exc:  # pragma: no cover - fail safe
            failures.append(f"{rel}: {exc}")

    if failures:
        print("CodeScene gate failed. Fix these issues before committing:", file=sys.stderr)
        for item in failures:
            print(f"  - {item}", file=sys.stderr)
        return 1

    print("CodeScene gate passed: staged code does not degrade and new code is perfect 10.0.")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except KeyboardInterrupt:
        raise SystemExit(1)
