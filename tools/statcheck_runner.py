#!/usr/bin/env python3
"""Run a statcheck executable against every preprocessed replay game."""

from __future__ import annotations

import argparse
import os
import re
import signal
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path

from rich.console import Console


GAME_ARCHIVE_PATTERN = re.compile(r"game_(\d+)\.scrd$")
DEFAULT_STATCHECK_TIMEOUT_SECONDS = 2.0
console = Console()


@dataclass(frozen=True)
class GameArchive:
    replay: str
    game_index: int
    path: Path


class StatcheckTimeoutError(RuntimeError):
    def __init__(self, timeout_seconds: float, output: str) -> None:
        self.output = output
        super().__init__(f"timed out after {timeout_seconds:g} seconds")


def find_game_archives(replay_root: Path) -> list[GameArchive]:
    if not replay_root.is_dir():
        raise RuntimeError(f"preprocessed replay directory is not a directory: {replay_root}")

    archives: list[GameArchive] = []
    for replay_dir in sorted(path for path in replay_root.iterdir() if path.is_dir()):
        for path in replay_dir.iterdir():
            if not path.is_file():
                continue
            match = GAME_ARCHIVE_PATTERN.fullmatch(path.name)
            if match:
                archives.append(GameArchive(replay_dir.name, int(match.group(1)), path))

    return sorted(archives, key=lambda archive: (archive.replay, archive.game_index))


def return_code_description(return_code: int) -> str:
    if return_code < 0:
        return f"terminated by signal {-return_code}"
    return f"exited with status {return_code}"


def timeout_output(exc: subprocess.TimeoutExpired) -> str:
    if isinstance(exc.output, bytes):
        return exc.output.decode(errors="replace")
    return exc.output or ""


def run_statcheck(command: list[str], timeout_seconds: float) -> subprocess.CompletedProcess[str]:
    process = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
        start_new_session=True,
    )
    try:
        output, _ = process.communicate(timeout=timeout_seconds)
    except subprocess.TimeoutExpired as exc:
        # 3SX can leave child processes behind. Kill the process group and
        # close our pipe instead of waiting for a surviving child to close it.
        try:
            os.killpg(process.pid, signal.SIGKILL)
        except ProcessLookupError:
            pass
        process.wait()
        if process.stdout is not None:
            process.stdout.close()
        raise StatcheckTimeoutError(timeout_seconds, timeout_output(exc)) from exc

    return subprocess.CompletedProcess(command, process.returncode, output)


def write_failure_report(
    report,
    archive: GameArchive,
    command: list[str],
    reason: str,
    output: str,
) -> None:
    report.write(f"{'=' * 80}\n")
    report.write(f"Replay: {archive.replay}\n")
    report.write(f"Game: game_{archive.game_index}\n")
    report.write(f"Archive: {archive.path}\n")
    report.write(f"Command: {' '.join(command)}\n")
    report.write(f"Result: {reason}\n")
    report.write("Process output:\n")
    report.write(output or "<no output>\n")
    if output and not output.endswith("\n"):
        report.write("\n")
    report.flush()


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("statcheck_executable", type=Path, help="statcheck executable")
    parser.add_argument("replay_dir", type=Path, help="directory containing <replay>/game_N.scrd archives")
    parser.add_argument(
        "--timeout",
        type=float,
        default=DEFAULT_STATCHECK_TIMEOUT_SECONDS,
        metavar="SECONDS",
        help=f"maximum statcheck time per game (default: {DEFAULT_STATCHECK_TIMEOUT_SECONDS:g})",
    )
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = parse_args(argv)
    if args.timeout <= 0:
        print("error: --timeout must be greater than zero", file=sys.stderr)
        return 1

    executable = args.statcheck_executable.resolve()
    if not executable.is_file():
        print(f"error: statcheck executable does not exist: {executable}", file=sys.stderr)
        return 1
    if not executable.stat().st_mode & 0o111:
        print(f"error: statcheck executable is not executable: {executable}", file=sys.stderr)
        return 1

    try:
        archives = find_game_archives(args.replay_dir)
    except RuntimeError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1
    if not archives:
        print(f"error: no game_N.scrd archives found under {args.replay_dir}", file=sys.stderr)
        return 1

    successes = 0
    timed_out = 0
    report = None
    report_path = None
    try:
        for index, archive in enumerate(archives, start=1):
            label = f"{archive.replay}/game_{archive.game_index}"
            command = [str(executable), "--ram-archive", str(archive.path), "--headless"]
            try:
                result = run_statcheck(command, args.timeout)
            except StatcheckTimeoutError as exc:
                timed_out += 1
                if report is None:
                    report = tempfile.NamedTemporaryFile(
                        mode="w", encoding="utf-8", prefix="statcheck-report-", suffix=".txt", delete=False
                    )
                    report_path = Path(report.name)
                write_failure_report(report, archive, command, str(exc), exc.output)
                console.print(f"[{index}/{len(archives)}] {label}: [red]✘[/red] ({exc})")
                continue
            except OSError as exc:
                if report is None:
                    report = tempfile.NamedTemporaryFile(
                        mode="w", encoding="utf-8", prefix="statcheck-report-", suffix=".txt", delete=False
                    )
                    report_path = Path(report.name)
                write_failure_report(report, archive, command, f"failed to start: {exc}", "")
                console.print(f"[{index}/{len(archives)}] {label}: [red]✘[/red] (failed to start)")
                continue

            if result.returncode == 0:
                successes += 1
                console.print(f"[{index}/{len(archives)}] {label}: [green]✔[/green]")
                continue

            reason = return_code_description(result.returncode)
            if report is None:
                report = tempfile.NamedTemporaryFile(
                    mode="w", encoding="utf-8", prefix="statcheck-report-", suffix=".txt", delete=False
                )
                report_path = Path(report.name)
            write_failure_report(report, archive, command, reason, result.stdout)
            console.print(f"[{index}/{len(archives)}] {label}: [red]✘[/red] ({reason})")
    finally:
        if report is not None:
            report.close()

    percentage = successes * 100 / len(archives)
    print(f"{successes}/{len(archives)} successful ({percentage:.1f}%)")
    if report_path is not None:
        print(f"Failure report: {report_path}")
    return 0 if successes == len(archives) else 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
