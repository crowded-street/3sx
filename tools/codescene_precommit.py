#!/usr/bin/env python3
"""Guard staged CodeScene quality.

New files must score a perfect 10.0. Existing files must not regress.
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
SUFFIXES = {".c", ".cpp", ".h", ".hpp", ".py"}
EXCLUDED = ("src/imgui/", "src/stb/", "src/argparse/", "src/bin2obj/")
SCORE_RE = re.compile(r"Code Health score:\s*([0-9.]+)")


def git(*args: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(["git", *args], cwd=REPO, capture_output=True, text=True, encoding="utf-8", errors="replace")


def cli_path() -> str:
    override = os.environ.get("CS_MCP_BINARY_PATH")
    if override:
        if not Path(override).is_file():
            raise SystemExit(
                "CodeScene CLI not found at CS_MCP_BINARY_PATH=\""
                + override
                + "\".\nInstall it with:\n  npm install -g @codescene/codehealth-mcp"
            )
        return override

    found = shutil.which("cs-mcp")
    if found:
        return found

    raise SystemExit(
        "CodeScene CLI is not installed or not on PATH.\n"
        "Install it with:\n  npm install -g @codescene/codehealth-mcp\n"
        "Then verify it with:\n  cs-mcp --help"
    )


def interesting(path: str) -> bool:
    if Path(path).suffix.lower() not in SUFFIXES:
        return False
    return not any(path.startswith(prefix) for prefix in EXCLUDED)


def is_code_file(path: str) -> bool:
    if not path:
        return False
    if Path(path).suffix.lower() not in SUFFIXES:
        return False
    return not any(path.startswith(prefix) for prefix in EXCLUDED)


def parse_staged_entries(raw: str) -> list[tuple[str, str | None]]:
    entries: list[tuple[str, str | None]] = []
    chunks = raw.split("\0")
    index = 0
    while index < len(chunks) - 1:
        status = chunks[index]
        if not status:
            index += 1
            continue

        if status.startswith(("R", "C")):
            old_path = chunks[index + 1]
            new_path = chunks[index + 2]
            entries.append((new_path, old_path))
            index += 3
            continue

        entries.append((chunks[index + 1], None))
        index += 2
    return entries


def staged_files() -> list[tuple[str, str | None]]:
    result = git("diff", "--cached", "--name-status", "-z", "--diff-filter=ACMR")
    if result.returncode != 0:
        raise SystemExit(result.stderr.strip() or "git diff failed")

    staged = [
        (path, previous_path)
        for path, previous_path in parse_staged_entries(result.stdout)
        if is_code_file(path)
    ]
    return sorted(staged, key=lambda item: item[0])


def show(ref: str, path: str) -> tuple[bool, str]:
    out = git("show", f"{ref}:{path}")
    if out.returncode != 0:
        return False, ""
    return True, out.stdout


def file_score_request(path: str) -> list[dict[str, object]]:
    return [
        {"jsonrpc": "2.0", "id": 1, "method": "initialize", "params": {"protocolVersion": "2024-11-05", "capabilities": {}, "clientInfo": {"name": "codescene-precommit", "version": "1.0.0"}}},
        {"jsonrpc": "2.0", "method": "notifications/initialized"},
        {"jsonrpc": "2.0", "id": 2, "method": "tools/call", "params": {"name": "code_health_score", "arguments": {"file_path": path}}},
    ]


def run_score_process(exe: str, payload: str) -> subprocess.CompletedProcess[str]:
    try:
        return subprocess.run(
            [exe],
            input=payload,
            capture_output=True,
            text=True,
            env={**os.environ, "CS_DISABLE_VERSION_CHECK": "1"},
            encoding="utf-8",
            errors="replace",
            timeout=30,
        )
    except subprocess.TimeoutExpired as exc:
        raise RuntimeError(f"CodeScene score request timed out after {exc.timeout}s") from exc


def run_score_request(exe: str, path: str) -> str:
    payload = "\n".join(json.dumps(item) for item in file_score_request(path)) + "\n"
    proc = run_score_process(exe, payload)
    if proc.returncode != 0 and not proc.stdout:
        raise RuntimeError((proc.stderr or proc.stdout or "CodeScene invocation failed").strip())
    return proc.stdout or ""


def parse_score(stdout: str, path: str) -> float:
    match = SCORE_RE.search(stdout)
    if not match:
        raise RuntimeError(f"No Code Health score returned for: {path}")
    return float(match.group(1))


def score(path: str, exe: str) -> float:
    return parse_score(run_score_request(exe, path), path)


def temp_file(content: str, suffix: str) -> str:
    handle = tempfile.NamedTemporaryFile("w", suffix=suffix, delete=False, encoding="utf-8")
    handle.write(content)
    handle.close()
    return handle.name


def cleanup_temp_files(paths: tuple[str, ...]) -> None:
    for candidate in paths:
        try:
            os.unlink(candidate)
        except FileNotFoundError:
            pass


def check_revised_file(path: str, exe: str, previous: str, staged: str) -> str | None:
    old_file = temp_file(previous, Path(path).name)
    new_file = temp_file(staged, Path(path).name)
    try:
        old_score = score(old_file, exe)
        new_score = score(new_file, exe)
    finally:
        cleanup_temp_files((old_file, new_file))

    if new_score < old_score - 1e-9:
        return f"{path}: Code Health degraded from {old_score:.2f} to {new_score:.2f}"
    return None


def check_new_file(path: str, exe: str, staged: str) -> str | None:
    new_file = temp_file(staged, Path(path).name)
    try:
        new_score = score(new_file, exe)
    finally:
        cleanup_temp_files((new_file,))

    if abs(new_score - 10.0) > 1e-9:
        return f"{path}: new code must score 10.0, got {new_score:.2f}"
    return None


def check(path: str, exe: str, previous_path: str | None = None) -> str | None:
    staged_exists, staged = show(":" + path, path)
    if not staged_exists:
        return None

    baseline_path = previous_path if previous_path is not None else path
    previous_exists, previous = show("HEAD", baseline_path)
    if not previous_exists:
        return check_new_file(path, exe, staged)
    return check_revised_file(path, exe, previous, staged)


def collect_failures(paths: list[tuple[str, str | None]], exe: str) -> list[str]:
    failures: list[str] = []
    for path, previous_path in paths:
        try:
            reason = check(path, exe, previous_path)
        except Exception as exc:  # pragma: no cover
            reason = f"{path}: {exc}"
        if reason:
            failures.append(reason)
    return failures


def report_failures(failures: list[str]) -> None:
    print(
        "CodeScene gate failed. Use the CodeScene MCP tools (code_health_review / code_health_score) to diagnose the issue before committing; do not guess at a fix.",
        file=sys.stderr,
    )
    for failure in failures:
        print(f"  - {failure}", file=sys.stderr)


def main() -> int:
    files = staged_files()
    if not files:
        return 0

    exe = cli_path()
    failures = collect_failures(files, exe)
    if failures:
        report_failures(failures)
        return 1

    print("CodeScene gate passed: staged code does not degrade and new code is perfect 10.0.")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except KeyboardInterrupt:
        raise SystemExit(1)
