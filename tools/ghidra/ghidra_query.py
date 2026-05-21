#!/usr/bin/env python3
"""Query a local Ghidra project through analyzeHeadless.

Examples:
  tools/ghidra/ghidra_query.py --program sfiii3 --target 0x06012345 --decompile
  tools/ghidra/ghidra_query.py --program sfiii3 --search Player
"""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[2]
SCRIPT_DIR = REPO_ROOT / "tools" / "ghidra"
DEFAULT_ENV_FILE = REPO_ROOT / ".env"


def load_env_file(path: Path) -> None:
    if not path.exists():
        return

    for line_number, raw_line in enumerate(path.read_text().splitlines(), start=1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        if line.startswith("export "):
            line = line[len("export ") :].strip()
        if "=" not in line:
            raise ValueError(f"{path}:{line_number}: expected KEY=VALUE")

        key, value = line.split("=", 1)
        key = key.strip()
        value = value.strip()
        if not key:
            raise ValueError(f"{path}:{line_number}: empty variable name")
        if len(value) >= 2 and value[0] == value[-1] and value[0] in ("'", '"'):
            value = value[1:-1]
        os.environ.setdefault(key, value)


def env_path(name: str) -> Path | None:
    value = os.environ.get(name)
    return Path(value).expanduser() if value else None


def parse_args() -> argparse.Namespace:
    env_file = Path(os.environ.get("GHIDRA_QUERY_ENV", DEFAULT_ENV_FILE)).expanduser()
    load_env_file(env_file)

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--project", type=Path, default=env_path("GHIDRA_PROJECT"), help="Path to a .gpr project file.")
    parser.add_argument(
        "--program",
        default=os.environ.get("GHIDRA_PROGRAM"),
        help="Ghidra project file/program name to process.",
    )
    parser.add_argument("--target", help="Address or symbol to inspect.")
    parser.add_argument("--search", help="Substring search across symbols.")
    parser.add_argument("--decompile", action="store_true", help="Include C decompiler output for the containing function.")
    parser.add_argument("--bytes", type=int, default=0, help="Read this many bytes from the target address.")
    parser.add_argument("--limit", type=int, default=25, help="Maximum symbol/xref rows to return.")
    parser.add_argument("--raw", action="store_true", help="Print compact raw JSON instead of pretty JSON.")
    parser.add_argument(
        "--headless",
        type=Path,
        default=env_path("GHIDRA_HEADLESS") or shutil.which("pyghidraRun"),
        help="Path to Ghidra's pyghidraRun launcher.",
    )
    parser.add_argument("--java-home", type=Path, default=env_path("JAVA_HOME"), help="JDK home for Ghidra.")
    return parser.parse_args()


def project_parts(project: Path) -> tuple[Path, str]:
    if project.suffix == ".gpr":
        return project.parent, project.stem
    return project.parent, project.name


def copy_project(project: Path, target_dir: Path) -> Path:
    project_location, project_name = project_parts(project)
    target_dir.mkdir(parents=True, exist_ok=True)
    copied_project = target_dir / f"{project_name}.gpr"
    copied_repo = target_dir / f"{project_name}.rep"
    source_repo = project_location / f"{project_name}.rep"

    shutil.copy2(project_location / f"{project_name}.gpr", copied_project)
    shutil.copytree(
        source_repo,
        copied_repo,
        ignore=shutil.ignore_patterns("*.lock", "*.lock~", "projectState"),
    )
    return copied_project


def run_query(args: argparse.Namespace, project: Path, out_path: Path) -> subprocess.CompletedProcess[str]:
    project_location, project_name = project_parts(project)
    env = os.environ.copy()
    if args.java_home:
        env["JAVA_HOME"] = str(args.java_home)

    command = [
        str(args.headless),
        "-H",
        str(project_location),
        project_name,
        "-readOnly",
        "-noanalysis",
        "-scriptPath",
        str(SCRIPT_DIR),
    ]
    if args.program:
        command.extend(["-process", args.program])
    else:
        command.append("-process")

    script_args = ["--out", str(out_path), "--limit", str(args.limit)]
    if args.target:
        script_args.extend(["--target", args.target])
    if args.search:
        script_args.extend(["--search", args.search])
    if args.decompile:
        script_args.append("--decompile")
    if args.bytes:
        script_args.extend(["--bytes", str(args.bytes)])

    command.extend(["-postScript", "GhidraQuery.py", *script_args])
    return subprocess.run(command, env=env, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


def main() -> int:
    args = parse_args()
    if not args.target and not args.search:
        print("error: provide --target, --search, or both", file=sys.stderr)
        return 2
    if not args.project:
        print("error: set GHIDRA_PROJECT in .env or pass --project", file=sys.stderr)
        return 2
    if not args.program:
        print("error: set GHIDRA_PROGRAM in .env or pass --program", file=sys.stderr)
        return 2
    if not args.headless:
        print("error: set GHIDRA_HEADLESS in .env or put pyghidraRun on PATH", file=sys.stderr)
        return 2

    with tempfile.TemporaryDirectory(prefix="ghidra-query-") as tmp:
        tmp_path = Path(tmp)
        out_path = tmp_path / "query.json"
        result = run_query(args, args.project, out_path)
        if "Unable to lock project" in result.stdout:
            copied_project = copy_project(args.project, tmp_path / "project-copy")
            out_path = tmp_path / "query-copy.json"
            result = run_query(args, copied_project, out_path)

        if result.returncode != 0:
            print(result.stdout, file=sys.stderr)
            return result.returncode
        if not out_path.exists():
            print(result.stdout, file=sys.stderr)
            print("error: Ghidra script did not produce JSON output", file=sys.stderr)
            return 1

        data = json.loads(out_path.read_text())
        print(json.dumps(data, indent=None if args.raw else 2, sort_keys=not args.raw))
        return 0


if __name__ == "__main__":
    raise SystemExit(main())
