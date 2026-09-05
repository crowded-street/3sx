"""Compare deterministic, headless gameplay traces from two 3SX builds.

Both executables receive the same generated controller inputs. Their normalized
rollback states are hashed after every frame and compared, making the first
behavioral divergence reproducible by seed and frame.
"""

import argparse
import csv
import shutil
import subprocess
import sys
import tempfile
from contextlib import contextmanager
from dataclasses import dataclass
from itertools import zip_longest
from pathlib import Path
from typing import Iterator

TOOLS_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS_DIR))

from stress_desync import run_dir_for  # noqa: E402

TRACE_NAME = "state-trace.csv"


@dataclass(frozen=True)
class TraceRun:
    executable: Path
    seed: int
    frames: int
    timeout: int


def run_trace(run: TraceRun, output_dir: Path) -> Path:
    output_dir.mkdir(parents=True)
    command = [
        str(run.executable.resolve()),
        "--stress",
        "--stress-seed",
        str(run.seed),
        "--stress-frames",
        str(run.frames),
        "--stress-out",
        str(output_dir.resolve()),
        "--no-sound",
    ]

    try:
        result = subprocess.run(
            command,
            cwd=run_dir_for(run.executable),
            stdout=subprocess.DEVNULL,
            stderr=subprocess.PIPE,
            text=True,
            errors="replace",
            timeout=run.timeout,
        )
    except subprocess.TimeoutExpired as error:
        raise RuntimeError(f"timed out after {run.timeout}s") from error

    trace = output_dir / TRACE_NAME

    if result.returncode != 0:
        raise RuntimeError(f"exited with status {result.returncode}: {result.stderr.strip()}")

    if not trace.exists():
        raise RuntimeError(f"did not produce {TRACE_NAME}")

    return trace


def read_trace(path: Path) -> list[tuple[int, str]]:
    with path.open(newline="", encoding="utf-8") as stream:
        rows = csv.DictReader(stream)
        return [(int(row["frame"]), row["checksum"]) for row in rows]


def first_difference(
    baseline: list[tuple[int, str]], candidate: list[tuple[int, str]]
) -> tuple[int, tuple[int, str] | None, tuple[int, str] | None] | None:
    for index, (baseline_row, candidate_row) in enumerate(zip_longest(baseline, candidate)):
        if baseline_row != candidate_row:
            return index, baseline_row, candidate_row

    return None


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Compare automated gameplay state between two 3SX builds.")
    parser.add_argument("baseline", type=Path, help="Executable built from the baseline branch.")
    parser.add_argument("candidate", type=Path, help="Executable built from the branch under test.")
    parser.add_argument("--seed", type=int, default=1, help="First deterministic input seed.")
    parser.add_argument("--seeds", type=int, default=1, help="Number of consecutive seeds to compare.")
    parser.add_argument("--frames", type=int, default=600, help="Gameplay frames per seed.")
    parser.add_argument("--timeout", type=int, default=300, help="Maximum seconds for each run.")
    parser.add_argument("--output", type=Path, help="Keep traces in this directory instead of a temporary one.")
    args = parser.parse_args()

    for executable in (args.baseline, args.candidate):
        if not executable.is_file():
            parser.error(f"executable not found: {executable}")

    return args


@contextmanager
def output_directory(requested: Path | None) -> Iterator[Path]:
    if requested is not None:
        requested.mkdir(parents=True, exist_ok=True)
        yield requested
        return

    with tempfile.TemporaryDirectory(prefix="3sx-replay-compare-") as temporary:
        yield Path(temporary)


def compare_seed(args: argparse.Namespace, output_root: Path, seed: int) -> int:
    seed_dir = output_root / f"seed-{seed}"

    if seed_dir.exists():
        shutil.rmtree(seed_dir)

    baseline_run = TraceRun(args.baseline, seed, args.frames, args.timeout)
    candidate_run = TraceRun(args.candidate, seed, args.frames, args.timeout)

    try:
        baseline_path = run_trace(baseline_run, seed_dir / "baseline")
        candidate_path = run_trace(candidate_run, seed_dir / "candidate")
    except RuntimeError as error:
        print(f"seed {seed}: ERROR: {error}")
        return 2

    baseline = read_trace(baseline_path)
    candidate = read_trace(candidate_path)
    difference = first_difference(baseline, candidate)

    if difference is None:
        print(f"seed {seed}: identical for {len(baseline)} saved states")
        return 0

    index, baseline_row, candidate_row = difference
    print(f"seed {seed}: DIVERGED at trace row {index}")
    print(f"  baseline:  {baseline_row}")
    print(f"  candidate: {candidate_row}")
    print(f"  traces: {seed_dir}")
    return 1


def compare_seeds(args: argparse.Namespace, output_root: Path) -> int:
    for seed in range(args.seed, args.seed + args.seeds):
        result = compare_seed(args, output_root, seed)

        if result != 0:
            return result

    return 0


def main() -> int:
    args = parse_args()

    with output_directory(args.output) as output_root:
        return compare_seeds(args, output_root)


if __name__ == "__main__":
    sys.exit(main())
