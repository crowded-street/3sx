"""Hunts for rollback desyncs and reports which state member caused them.

Each run starts a GekkoNet stress session, which re-simulates the last
--check-distance frames every update and compares the resulting checksums
against the first pass. When they disagree, the game dumps both simulations of
that frame into states/, and compare_states.py resolves the differing bytes to
struct members.

    python3 tools/stress_desync.py --seeds 20
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
NETPLAY_OBJ = Path("CMakeFiles/3sx.dir/src/platform/netplay/netplay.c.obj")

# build/stress is the optimised harness build; a Debug build works but is about
# eight times slower, since every re-simulated frame copies and hashes the state.
BUILD_DIRS = [REPO_ROOT / "build" / "stress", REPO_ROOT / "build"]


def exe_names() -> list[str]:
    if sys.platform == "win32":
        return ["3sx.exe"]

    if sys.platform == "darwin":
        return ["3SX.app/Contents/MacOS/3SX", "3sx"]

    return ["3sx"]


def find_exe(build: Path) -> Path | None:
    for name in exe_names():
        candidate = build / "application" / "bin" / name

        if candidate.exists():
            return candidate

    return None


def default_exe() -> Path | None:
    """THREESX_EXE wins, as it does for tools/run-netplay-local.sh."""
    override = os.environ.get("THREESX_EXE")

    if override:
        return Path(override)

    for build in BUILD_DIRS:
        found = find_exe(build)

        if found:
            return found

    return None

DESYNC_RE = re.compile(r"desync at frame (-?\d+) after (\d+) frames")
TRACE_NAME = "stress-trace.log"

# compare_states.py shells out to dwarfdump; MSYS2 only ships llvm-dwarfdump.
DWARFDUMP_NAMES = ["dwarfdump", "llvm-dwarfdump"]


def prepare_run_dir(bin_dir: Path, seed: int) -> Path:
    """Each seed gets its own directory so runs can go in parallel."""
    run_dir = bin_dir / "stress-runs" / f"seed-{seed}"

    if run_dir.exists():
        shutil.rmtree(run_dir)

    run_dir.mkdir(parents=True)

    return run_dir


def find_obj(exe: Path) -> Path | None:
    """The object file sits in the build tree the executable was installed from,
    so walk up from it before falling back to the usual build directories."""
    candidates = [parent / NETPLAY_OBJ for parent in exe.resolve().parents]
    candidates += [build / NETPLAY_OBJ for build in BUILD_DIRS]

    for candidate in candidates:
        if candidate.exists():
            return candidate

    return None


def dwarfdump_env() -> dict:
    """compare_states.py reads DWARFDUMP, so resolve the tool off PATH for it."""
    env = dict(os.environ)

    if "DWARFDUMP" in env:
        return env

    for name in DWARFDUMP_NAMES:
        found = shutil.which(name)

        if found:
            env["DWARFDUMP"] = found
            break

    return env


def start_session(exe: Path, run_dir: Path, seed: int, frames: int, check_distance: int) -> subprocess.Popen:
    command = [
        str(exe),
        "--stress",
        "--stress-seed", str(seed),
        "--stress-out", str(run_dir),
        "--no-sound",
    ]

    if frames:
        command += ["--stress-frames", str(frames)]

    if check_distance:
        command += ["--stress-check-distance", str(check_distance)]

    return subprocess.Popen(command, cwd=exe.parent, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def collect(process: subprocess.Popen, run_dir: Path, timeout: int) -> tuple[str, bool]:
    """Waits for one session and returns its trace."""
    timed_out = False

    try:
        process.wait(timeout=timeout)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait()
        timed_out = True

    trace = run_dir / TRACE_NAME

    return (trace.read_text(encoding="utf-8", errors="replace") if trace.exists() else "", timed_out)


def abandon(running: list) -> None:
    """One desync is enough to work with, so the rest of the wave is wasted."""
    for _, _, process in running:
        process.kill()

    for _, _, process in running:
        process.wait()


def explain(obj: Path, working_dir: Path) -> str:
    result = subprocess.run(
        [sys.executable, str(REPO_ROOT / "tools" / "compare_states.py"), str(obj)],
        cwd=working_dir,
        capture_output=True,
        text=True,
        errors="replace",
        env=dwarfdump_env(),
    )

    if result.returncode != 0:
        return f"compare_states.py failed: {result.stderr.strip()}"

    return result.stdout.strip() or "No byte differences found in the dumped states."


def main():
    parser = argparse.ArgumentParser(description="Find rollback desyncs and explain them.")
    parser.add_argument("--exe", type=Path, help="Path to a netplay-enabled 3sx build.")
    parser.add_argument("--obj", type=Path, help="Object file to read State debug info from.")
    parser.add_argument("--seed", type=int, default=1, help="First seed to run.")
    parser.add_argument("--seeds", type=int, default=1, help="How many consecutive seeds to run.")
    parser.add_argument("--frames", type=int, default=0, help="Frame cap per seed (0 runs until a desync).")
    parser.add_argument("--check-distance", type=int, default=0, help="Rollback depth (0 uses the game's default).")
    parser.add_argument("--timeout", type=int, default=300, help="Seconds to allow per seed when uncapped.")
    parser.add_argument("--jobs", type=int, default=1, help="How many sessions to run at once.")
    parser.add_argument("--keep-going", action="store_true", help="Keep trying seeds after the first desync.")
    args = parser.parse_args()

    args.exe = args.exe or default_exe()

    if args.exe is None:
        print("No build found. Pass --exe or set THREESX_EXE.")
        return 1

    args.obj = args.obj or find_obj(args.exe)

    if args.obj is None:
        print(f"Could not find {NETPLAY_OBJ.name} near {args.exe}. Pass --obj.")
        return 1

    if not args.exe.exists():
        print(f"No build at {args.exe}. Build and install first.")
        return 1

    if not args.obj.exists():
        print(f"No object file at {args.obj}. Build first, or pass --obj.")
        return 1

    # A stress frame costs about check_distance extra simulated frames, so this is
    # far slower than realtime. Without a frame cap, cap the wall clock instead.
    timeout = int(args.frames / 60 * 6) + 120 if args.frames else args.timeout
    failures = 0

    seeds = list(range(args.seed, args.seed + args.seeds))
    cap = f"{args.frames} frames" if args.frames else f"until desync, {timeout}s cap"
    jobs = max(1, args.jobs)

    for wave_start in range(0, len(seeds), jobs):
        wave = seeds[wave_start:wave_start + jobs]
        print(f"=== seeds {wave[0]}-{wave[-1]} ({cap}) ===", flush=True)

        running = []

        for seed in wave:
            run_dir = prepare_run_dir(args.exe.parent, seed)
            running.append((seed, run_dir, start_session(args.exe, run_dir, seed, args.frames, args.check_distance)))

        for index, (seed, run_dir, process) in enumerate(running):
            output, timed_out = collect(process, run_dir, timeout)

            if timed_out:
                print(f"seed {seed}: timed out after {timeout}s.")
            elif "exiting:" not in output:
                print(f"seed {seed}: exited without logging a reason - it may have crashed.")

            desyncs = DESYNC_RE.findall(output)

            if not desyncs:
                print(f"seed {seed}: no desync.")
                continue

            failures += 1
            frames = ", ".join(frame for frame, _ in desyncs)
            print(f"seed {seed}: desync at frame(s) {frames}")
            print(explain(args.obj, run_dir))

            if not args.keep_going:
                abandon(running[index + 1:])
                break

        if failures and not args.keep_going:
            break

    print()
    print(f"{failures} seed(s) desynced.")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
