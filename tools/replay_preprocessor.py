#!/usr/bin/env python3
"""Run Fightcade replays and collect the SCRD archives produced by FBNeo."""

from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
import tempfile
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Any


@dataclass
class ReplayResult:
    replay: str
    status: str
    game: str | None = None
    archives: list[str] | None = None
    error: str | None = None


def find_replays(replay_root: Path) -> list[Path]:
    if not replay_root.is_dir():
        raise RuntimeError(f"replay directory is not a directory: {replay_root}")

    return sorted(
        path
        for path in replay_root.iterdir()
        if path.is_dir() and (path / "savestate").is_file() and (path / "inputs").is_file()
    )


def replay_game(replay_dir: Path, game_override: str | None) -> str:
    if game_override:
        return game_override

    summary_path = replay_dir / "summary.json"
    try:
        summary: Any = json.loads(summary_path.read_text(encoding="utf-8"))
    except FileNotFoundError as exc:
        raise RuntimeError(f"missing {summary_path}; pass --game to set the game ID") from exc
    except json.JSONDecodeError as exc:
        raise RuntimeError(f"invalid JSON in {summary_path}: {exc}") from exc

    game = summary.get("game") if isinstance(summary, dict) else None
    if not isinstance(game, str) or not game:
        raise RuntimeError(f"missing game in {summary_path}; pass --game to set the game ID")
    return game


GAME_ARCHIVE_PATTERN = re.compile(r"game_\d+\.scrd$")


def game_archives(archive_root: Path) -> list[Path]:
    return sorted(
        path
        for path in archive_root.iterdir()
        if path.is_file() and GAME_ARCHIVE_PATTERN.fullmatch(path.name)
    )


def run_replay(
    runner: Path,
    replay_dir: Path,
    game: str,
    archive_root: Path,
) -> None:
    command = [
        str(runner),
        game,
        "-replay-state",
        str(replay_dir / "savestate"),
        "-replay-inputs",
        str(replay_dir / "inputs"),
        "-headless",
        "-dump-ram-path",
        str(archive_root),
    ]
    subprocess.run(command, check=True)


def process_replay(
    runner: Path,
    replay_dir: Path,
    output_dir: Path,
    game_override: str | None,
    force: bool,
    temp_root: Path,
) -> ReplayResult:
    game = replay_game(replay_dir, game_override)
    archive_root = temp_root / replay_dir.name
    archive_root.mkdir(parents=True)
    run_replay(runner, replay_dir, game, archive_root)

    produced_archives = game_archives(archive_root)
    if not produced_archives:
        raise RuntimeError("replay runner produced no game_N.scrd archives")

    archives: list[str] = []
    for archive_path in produced_archives:
        output_path = output_dir / replay_dir.name / archive_path.name
        if output_path.exists() and not force:
            raise RuntimeError(f"output file already exists: {output_path} (use --force to overwrite)")
        output_path.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(archive_path, output_path)
        archives.append(str(output_path))
        print(f"  wrote {output_path}")

    return ReplayResult(replay=replay_dir.name, status="archived", game=game, archives=archives)


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("replay_dir", type=Path, help="directory containing replay subdirectories")
    parser.add_argument("output_dir", type=Path, help="directory for <replay>/game_N.scrd archives")
    parser.add_argument("--runner", type=Path, required=True, help="FBNeo replay runner executable")
    parser.add_argument("--game", help="override the game ID instead of reading each summary.json")
    parser.add_argument("-f", "--force", action="store_true", help="overwrite existing SCRD archives")
    parser.add_argument("--fail-fast", action="store_true", help="stop at the first replay failure")
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = parse_args(argv)
    runner = args.runner.resolve()
    if not runner.is_file():
        print(f"error: replay runner does not exist: {runner}", file=sys.stderr)
        return 1
    if not runner.stat().st_mode & 0o111:
        print(f"error: replay runner is not executable: {runner}", file=sys.stderr)
        return 1

    try:
        replays = find_replays(args.replay_dir)
    except RuntimeError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1
    if not replays:
        print(f"error: no replay directories with both savestate and inputs under {args.replay_dir}", file=sys.stderr)
        return 1

    args.output_dir.mkdir(parents=True, exist_ok=True)
    results: list[ReplayResult] = []
    for index, replay_dir in enumerate(replays, start=1):
        print(f"[{index}/{len(replays)}] {replay_dir.name}")
        try:
            # Keep the runner's archives isolated until it completes, so a
            # failed replay cannot leave partial output in the destination.
            with tempfile.TemporaryDirectory(prefix="fbneo-scrd-archives-") as temp_name:
                result = process_replay(
                    runner,
                    replay_dir,
                    args.output_dir,
                    args.game,
                    args.force,
                    Path(temp_name),
                )
        except (OSError, RuntimeError, subprocess.CalledProcessError) as exc:
            result = ReplayResult(replay=replay_dir.name, status="error", error=str(exc))
            print(f"  error: {exc}", file=sys.stderr)
            results.append(result)
            if args.fail_fast:
                break
        else:
            results.append(result)

    manifest_path = args.output_dir / "bulk_ram_dump_manifest.json"
    manifest_path.write_text(
        json.dumps({"results": [asdict(result) for result in results]}, indent=2) + "\n",
        encoding="utf-8",
    )
    archived = sum(result.status == "archived" for result in results)
    failed = sum(result.status == "error" for result in results)
    print(f"wrote {manifest_path}; {archived} archived, {failed} failed")
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
