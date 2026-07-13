#!/usr/bin/env python3
"""Run Fightcade replays into temporary RAM dumps and package them as SCRD files."""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
import tempfile
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Any

from compress_ram_dumps import compress_ram_dumps


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


def game_dump_dirs(dump_root: Path) -> list[Path]:
    return sorted(
        path
        for path in dump_root.iterdir()
        if path.is_dir() and path.name.startswith("game_")
    )


def run_replay(
    runner: Path,
    replay_dir: Path,
    game: str,
    dump_root: Path,
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
        str(dump_root),
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
    dump_root = temp_root / replay_dir.name
    dump_root.mkdir(parents=True)
    run_replay(runner, replay_dir, game, dump_root)

    dumps = game_dump_dirs(dump_root)
    if not dumps:
        raise RuntimeError("replay runner produced no game_N RAM-dump directories")

    archives: list[str] = []
    for dump_dir in dumps:
        output_path = output_dir / replay_dir.name / f"{dump_dir.name}.scrd"
        frame_count = compress_ram_dumps(dump_dir, output_path, force=force)
        archives.append(str(output_path))
        print(f"  wrote {output_path} ({frame_count} frames)")

    return ReplayResult(replay=replay_dir.name, status="compressed", game=game, archives=archives)


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
            # A replay can generate tens of gigabytes of raw frame dumps. Keep
            # only one replay's dumps at a time, then delete them as soon as its
            # game_N archives have been written.
            with tempfile.TemporaryDirectory(prefix="fbneo-ram-dumps-") as temp_name:
                result = process_replay(
                    runner, replay_dir, args.output_dir, args.game, args.force, Path(temp_name)
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
    failed = sum(result.status == "error" for result in results)
    print(f"wrote {manifest_path}; {len(results) - failed} compressed, {failed} failed")
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
