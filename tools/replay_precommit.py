"""Run a bounded deterministic gameplay comparison before relevant commits."""

import os
import subprocess
import sys
import tempfile
from pathlib import Path

from compare_stress_replays import TraceRun, first_difference, read_trace, run_trace

REPO_ROOT = Path(__file__).resolve().parent.parent
GAME_SOURCE_PREFIX = "src/sf33rd/Source/Game/"
SOURCE_SUFFIXES = {".c", ".h"}
DEFAULT_FRAMES = 120
DEFAULT_TIMEOUT = 180


def staged_game_sources() -> list[str]:
    result = subprocess.run(
        ["git", "diff", "--cached", "--name-only", "--diff-filter=ACMR"],
        cwd=REPO_ROOT,
        check=True,
        capture_output=True,
        text=True,
    )
    return [
        path
        for path in result.stdout.splitlines()
        if path.startswith(GAME_SOURCE_PREFIX) and Path(path).suffix in SOURCE_SUFFIXES
    ]


def executable_in(build_dir: Path) -> Path:
    if sys.platform == "darwin":
        return build_dir / "3SX.app" / "Contents" / "MacOS" / "3SX"

    executable_name = "3sx.exe" if sys.platform == "win32" else "3sx"
    return build_dir / executable_name


def configured_path(variable: str, default: Path) -> Path:
    value = os.environ.get(variable)
    return Path(value) if value else default


def replay_required(changed: list[str]) -> bool:
    return bool(changed) or os.environ.get("THREESX_REPLAY_GUARD_FORCE") == "1"


def build_candidate(build_dir: Path) -> None:
    command = ["cmake", "--build", str(build_dir), "--parallel", "--config", "Debug"]
    subprocess.run(command, cwd=REPO_ROOT, check=True)


def missing_build_message(baseline: Path, candidate: Path) -> str:
    return (
        "Replay gate needs Debug builds with deterministic tracing.\n"
        f"  baseline missing: {baseline}\n"
        f"  candidate missing: {candidate}\n"
        "Create build-replay-main from main and build-replay from the working branch, "
        "or set THREESX_REPLAY_GUARD_BASELINE, THREESX_REPLAY_GUARD_CANDIDATE, "
        "and THREESX_REPLAY_GUARD_BUILD_DIR."
    )


def compare_builds(baseline: Path, candidate: Path) -> int:
    with tempfile.TemporaryDirectory(prefix="3sx-precommit-replay-") as directory:
        output = Path(directory)
        baseline_trace = run_trace(TraceRun(baseline, 1, DEFAULT_FRAMES, DEFAULT_TIMEOUT), output / "baseline")
        candidate_trace = run_trace(TraceRun(candidate, 1, DEFAULT_FRAMES, DEFAULT_TIMEOUT), output / "candidate")
        baseline_rows = read_trace(baseline_trace)
        candidate_rows = read_trace(candidate_trace)
        difference = first_difference(baseline_rows, candidate_rows)

    if difference is None:
        print(f"Replay gate passed: {len(baseline_rows)} deterministic gameplay states are identical.")
        return 0

    index, baseline_row, candidate_row = difference
    print("Replay gate failed: deterministic gameplay diverged.", file=sys.stderr)
    print(f"  trace row: {index}", file=sys.stderr)
    print(f"  baseline:  {baseline_row}", file=sys.stderr)
    print(f"  candidate: {candidate_row}", file=sys.stderr)
    return 1


def main() -> int:
    changed = staged_game_sources()

    if not replay_required(changed):
        print("Replay gate skipped: no staged gameplay source changes.")
        return 0

    candidate_build = configured_path("THREESX_REPLAY_GUARD_BUILD_DIR", REPO_ROOT / "build-replay")
    baseline = configured_path(
        "THREESX_REPLAY_GUARD_BASELINE", executable_in(REPO_ROOT / "build-replay-main")
    )
    candidate = configured_path("THREESX_REPLAY_GUARD_CANDIDATE", executable_in(candidate_build))

    try:
        build_candidate(candidate_build)
    except (OSError, subprocess.CalledProcessError) as error:
        print(f"Replay gate failed to build candidate: {error}", file=sys.stderr)
        return 2

    if not baseline.is_file() or not candidate.is_file():
        print(missing_build_message(baseline, candidate), file=sys.stderr)
        return 2

    try:
        return compare_builds(baseline, candidate)
    except RuntimeError as error:
        print(f"Replay gate failed to run: {error}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
