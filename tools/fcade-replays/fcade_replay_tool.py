#!/usr/bin/env python3
"""Fightcade replay downloader/parser (work-in-progress reverse engineering helper)."""

from __future__ import annotations

import argparse
import json
import os
import socket
import struct
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
import zlib
from dataclasses import dataclass
from pathlib import Path
from typing import Any


DEFAULT_HOST = "ggpo.fightcade.com"
DEFAULT_PORT = 7100
DEFAULT_API_URL = "https://www.fightcade.com/api/"
DEFAULT_COOKIE_ENV = "FCADE_COOKIE"
DEFAULT_USER_AGENT = (
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 "
    "(KHTML, like Gecko) Chrome/149.0.0.0 Safari/537.36"
)


@dataclass
class ReplayTarget:
    emulator: str
    game: str
    token: str
    port: int


@dataclass
class ListedReplay:
    target: ReplayTarget
    source: dict[str, Any]


class FightcadeApiError(RuntimeError):
    pass


def _u32be(value: int) -> bytes:
    return struct.pack(">I", value & 0xFFFFFFFF)


def _i32be_from(buf: bytes, off: int = 0) -> int:
    return struct.unpack_from(">i", buf, off)[0]


def _u32be_from(buf: bytes, off: int = 0) -> int:
    return struct.unpack_from(">I", buf, off)[0]


def _recv_exact(sock: socket.socket, size: int) -> bytes:
    out = bytearray()
    while len(out) < size:
        chunk = sock.recv(size - len(out))
        if not chunk:
            raise EOFError("socket closed while reading")
        out.extend(chunk)
    return bytes(out)


def recv_frame(sock: socket.socket) -> bytes:
    length = _u32be_from(_recv_exact(sock, 4))
    return _recv_exact(sock, length)


def parse_fcade_url(url: str) -> ReplayTarget:
    parsed = urllib.parse.urlparse(url)
    if parsed.scheme != "fcade":
        raise ValueError(f"expected fcade:// URL, got: {url}")
    if parsed.netloc != "stream":
        raise ValueError(f"expected fcade://stream/... URL, got netloc={parsed.netloc!r}")

    parts = parsed.path.strip("/").split("/")
    if len(parts) != 3:
        raise ValueError(f"unexpected fcade path format: {parsed.path!r}")

    emulator, game, tail = parts
    if "," not in tail:
        raise ValueError(f"expected '<token>,<port>' in path tail, got: {tail!r}")
    token, port_text = tail.rsplit(",", 1)

    try:
        port = int(port_text)
    except ValueError as exc:
        raise ValueError(f"invalid port in fcade URL: {port_text!r}") from exc

    return ReplayTarget(emulator=emulator, game=game, token=token, port=port)


def _json_walk(value: Any) -> list[Any]:
    out = [value]
    if isinstance(value, dict):
        for child in value.values():
            out.extend(_json_walk(child))
    elif isinstance(value, list):
        for child in value:
            out.extend(_json_walk(child))
    return out


def _stream_token_from_quark_id(token: str) -> str:
    if "." in token:
        return token
    return f"{token}.7"


def _listed_replay_from_dict(data: dict[str, Any], default_game: str, default_emulator: str) -> ListedReplay | None:
    quark_id = data.get("quarkid")
    if not isinstance(quark_id, str) or not quark_id:
        return None
    token = _stream_token_from_quark_id(quark_id)

    game = data.get("gameid")
    if not isinstance(game, str) or not game:
        game = default_game

    emulator = data.get("emulator")
    if not isinstance(emulator, str) or not emulator:
        emulator = default_emulator

    port = DEFAULT_PORT
    return ListedReplay(target=ReplayTarget(emulator=emulator, game=game, token=token, port=port), source=data)


def extract_listed_replays(api_response: Any, default_game: str, default_emulator: str) -> list[ListedReplay]:
    seen: set[tuple[str, str, int]] = set()
    replays: list[ListedReplay] = []
    for value in _json_walk(api_response):
        if not isinstance(value, dict):
            continue
        replay = _listed_replay_from_dict(value, default_game=default_game, default_emulator=default_emulator)
        if not replay:
            continue
        key = (replay.target.game, replay.target.token, replay.target.port)
        if key in seen:
            continue
        seen.add(key)
        replays.append(replay)
    return replays


def search_quarks(
    api_url: str,
    gameid: str,
    offset: int,
    limit: int,
    best: bool,
    since: int | None,
    cookie: str | None,
    timeout: float,
    user_agent: str,
) -> Any:
    payload: dict[str, Any] = {
        "req": "searchquarks",
        "offset": offset,
        "limit": limit,
        "gameid": gameid,
    }
    if best:
        payload["best"] = True
    if since is not None:
        payload["since"] = since

    body = json.dumps(payload).encode("utf-8")
    headers = {
        "Accept": "application/json, text/plain, */*",
        "Content-Type": "application/json;charset=UTF-8",
        "Origin": "https://www.fightcade.com",
        "Referer": f"https://www.fightcade.com/game/{gameid}",
        "User-Agent": user_agent,
    }
    cookie_value = cookie or os.environ.get(DEFAULT_COOKIE_ENV)
    if cookie_value:
        headers["Cookie"] = cookie_value

    req = urllib.request.Request(api_url, data=body, headers=headers, method="POST")
    try:
        with urllib.request.urlopen(req, timeout=timeout) as response:
            raw = response.read()
    except urllib.error.HTTPError as exc:
        if exc.code == 403:
            raise FightcadeApiError(
                "Fightcade API returned 403 Forbidden. The replay listing endpoint usually needs "
                "a current Cloudflare clearance cookie; pass --cookie 'cf_clearance=...' or set "
                f"{DEFAULT_COOKIE_ENV}."
            ) from exc
        raise FightcadeApiError(f"Fightcade API request failed with HTTP {exc.code}: {exc.reason}") from exc
    except urllib.error.URLError as exc:
        raise FightcadeApiError(f"Fightcade API request failed: {exc.reason}") from exc
    return json.loads(raw.decode("utf-8"))


def do_handshake(sock: socket.socket, token: str, send_delay_ms: float) -> list[bytes]:
    token_bytes = token.encode("utf-8")
    received: list[bytes] = []
    send_delay_s = max(0.0, send_delay_ms) / 1000.0

    def send_raw(payload: bytes) -> None:
        sock.sendall(payload)
        if send_delay_s > 0:
            time.sleep(send_delay_s)

    def pack_u32(*values: int) -> bytes:
        return b"".join(_u32be(v) for v in values)

    # Observed client handshake sequence from capture. Important: these are raw
    # words/chunks over TCP, not "frame(payload)" sends.
    send_raw(pack_u32(0x14))
    send_raw(pack_u32(1, 0))
    send_raw(pack_u32(0, 0x1D, 1))
    # Server responds with a type=1 ack before stream/token commands.
    try:
        received.append(recv_frame(sock))
    except Exception:
        # Keep going even if ack read fails; some environments may coalesce timings.
        pass
    send_raw(pack_u32(0x20))
    send_raw(pack_u32(2))
    send_raw(pack_u32(len(token_bytes), len(token_bytes)) + token_bytes)
    send_raw(pack_u32(0x20))
    send_raw(pack_u32(3))
    send_raw(pack_u32(0x0C))
    send_raw(pack_u32(len(token_bytes)) + token_bytes)
    return received


def _parse_metadata_type3(payload: bytes) -> dict:
    # payload layout seen so far:
    # int32 type=3, int32 ?, int32 ?, int32 lenA, bytesA, int32 lenB, bytesB, ...
    out = {"type": 3}
    if len(payload) < 16:
        out["raw_hex"] = payload.hex()
        return out

    out["field_4"] = _u32be_from(payload, 4)
    out["field_8"] = _u32be_from(payload, 8)
    off = 12
    names: list[str] = []

    for _ in range(2):
        if off + 4 > len(payload):
            break
        n = _u32be_from(payload, off)
        off += 4
        if off + n > len(payload):
            break
        raw = payload[off : off + n]
        off += n
        names.append(raw.decode("utf-8", errors="replace"))

    out["strings"] = names
    out["trailing_hex"] = payload[off:].hex()
    return out


def _parse_minus13(payload: bytes) -> dict:
    # int32 type=-13, u32 record_size, u32 record_count, records...
    if len(payload) < 12:
        return {"type": -13, "error": "short payload", "raw_hex": payload.hex()}

    record_size = _u32be_from(payload, 4)
    record_count = _u32be_from(payload, 8)
    body = payload[12:]
    expected = record_size * record_count
    return {
        "type": -13,
        "record_size": record_size,
        "record_count": record_count,
        "body_len": len(body),
        "expected_body_len": expected,
        "body_matches": len(body) == expected,
        "records_preview_hex": body[: min(len(body), record_size * min(record_count, 3))].hex(),
    }


def _parse_minus12(payload: bytes) -> dict:
    # int32 type=-12, u32 field_4 (often uncompressed size), then compressed bytes.
    if len(payload) < 8:
        return {"type": -12, "error": "short payload", "raw_hex": payload.hex()}

    field_4 = _u32be_from(payload, 4)
    compressed = payload[8:]
    out = {
        "type": -12,
        "field_4": field_4,
        "compressed_len": len(compressed),
        "compressed_starts": compressed[:8].hex(),
        "zlib_header_like": len(compressed) >= 2 and compressed[0] == 0x78,
    }

    try:
        decompressed = zlib.decompress(compressed)
        out["decompressed_len"] = len(decompressed)
        out["decompressed_starts"] = decompressed[:32].hex()
    except Exception as exc:  # noqa: BLE001
        out["decompress_error"] = str(exc)

    return out


def parse_server_message(payload: bytes) -> dict:
    if len(payload) < 4:
        return {"type": None, "error": "short payload", "payload_len": len(payload)}

    msg_type = _i32be_from(payload, 0)
    if msg_type == 3:
        return _parse_metadata_type3(payload)
    if msg_type == -13:
        return _parse_minus13(payload)
    if msg_type == -12:
        return _parse_minus12(payload)

    return {
        "type": msg_type,
        "payload_len": len(payload),
        "payload_starts": payload[:64].hex(),
    }


def download_replay(
    target: ReplayTarget,
    host: str,
    out_dir: Path,
    timeout: float,
    idle_timeout: float,
    max_idle_timeouts: int,
    max_frames: int,
    local_port: int,
    send_delay_ms: float,
) -> dict:
    out_dir.mkdir(parents=True, exist_ok=True)

    frames_bin = out_dir / "frames.bin"
    summary_json = out_dir / "summary.json"
    inputs_path = out_dir / "inputs"
    savestate_path = out_dir / "savestate"

    messages: list[dict] = []
    count = 0

    used_local_port = local_port
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(timeout)
    if local_port > 0:
        try:
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.bind(("", local_port))
        except OSError:
            sock.close()
            used_local_port = 0
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(timeout)
    try:
        sock.connect((host, target.port))
    except (TimeoutError, socket.timeout):
        sock.close()
        if local_port <= 0:
            raise
        # Fallback: when repeatedly testing, forcing the same source port can time out.
        used_local_port = 0
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(timeout)
        sock.connect((host, target.port))
    with sock:
        sock.settimeout(idle_timeout)
        handshake_messages = do_handshake(sock, target.token, send_delay_ms=send_delay_ms)
        idle_hits = 0

        with frames_bin.open("wb") as fw, inputs_path.open("wb") as inputs_f:
            savestate_written = False
            for payload in handshake_messages:
                fw.write(_u32be(len(payload)))
                fw.write(payload)
                info = parse_server_message(payload)
                info.update({"index": count, "length": len(payload), "stage": "handshake"})
                messages.append(info)
                count += 1

            while count < max_frames:
                try:
                    payload = recv_frame(sock)
                except socket.timeout:
                    idle_hits += 1
                    if idle_hits >= max_idle_timeouts:
                        break
                    continue
                except EOFError:
                    break
                idle_hits = 0

                fw.write(_u32be(len(payload)))
                fw.write(payload)

                info = parse_server_message(payload)
                info.update({"index": count, "length": len(payload)})
                messages.append(info)

                msg_type = info.get("type")
                if msg_type == -12:
                    chunk = payload[8:]
                    try:
                        raw = zlib.decompress(chunk)
                        if not savestate_written:
                            savestate_path.write_bytes(raw)
                            savestate_written = True
                    except Exception:
                        pass
                elif msg_type == -13:
                    body = payload[12:]
                    expected = info.get("expected_body_len")
                    if isinstance(expected, int) and expected >= 0 and len(body) >= expected:
                        inputs_f.write(body[:expected])
                        if len(body) != expected:
                            info["inputs_trailer_len"] = len(body) - expected
                    else:
                        # Fallback if fields are missing/corrupt: preserve prior behavior.
                        inputs_f.write(body)

                count += 1

    summary = {
        "downloaded_at_unix": int(time.time()),
        "host": host,
        "port": target.port,
        "emulator": target.emulator,
        "game": target.game,
        "token": target.token,
        "local_port": used_local_port,
        "send_delay_ms": send_delay_ms,
        "idle_timeout": idle_timeout,
        "max_idle_timeouts": max_idle_timeouts,
        "messages": messages,
    }
    summary_json.write_text(json.dumps(summary, indent=2), encoding="utf-8")
    return summary


def _target_from_args(args: argparse.Namespace) -> ReplayTarget:
    if args.fcade_url:
        return parse_fcade_url(args.fcade_url)

    if not args.game or not args.token:
        raise ValueError("provide either --fcade-url or both --game and --token")

    return ReplayTarget(
        emulator=args.emulator,
        game=args.game,
        token=args.token,
        port=args.port,
    )


def cmd_download(args: argparse.Namespace) -> int:
    target = _target_from_args(args)

    out_dir = Path(args.out_dir)
    if args.auto_dir:
        safe_token = target.token.replace("/", "_")
        out_dir = out_dir / f"{target.game}-{safe_token}"

    summary = download_replay(
        target=target,
        host=args.host,
        out_dir=out_dir,
        timeout=args.timeout,
        idle_timeout=args.idle_timeout,
        max_idle_timeouts=args.max_idle_timeouts,
        max_frames=args.max_frames,
        local_port=args.local_port,
        send_delay_ms=args.send_delay_ms,
    )

    print(json.dumps({"out_dir": str(out_dir), "message_count": len(summary["messages"])}, indent=2))
    return 0


def _safe_token(token: str) -> str:
    return token.replace("/", "_").replace("\\", "_").replace(":", "_")


def fetch_replay_list(args: argparse.Namespace) -> list[ListedReplay]:
    if args.count < 1:
        return []
    if args.page_size < 1:
        raise ValueError("--page-size must be at least 1")

    replays: list[ListedReplay] = []
    seen: set[tuple[str, str, int]] = set()
    offset = args.offset

    while len(replays) < args.count:
        page_limit = min(args.page_size, args.count - len(replays))
        response = search_quarks(
            api_url=args.api_url,
            gameid=args.gameid,
            offset=offset,
            limit=page_limit,
            best=args.best,
            since=args.since,
            cookie=args.cookie,
            timeout=args.api_timeout,
            user_agent=args.user_agent,
        )
        page_replays = extract_listed_replays(response, default_game=args.gameid, default_emulator=args.emulator)
        if not page_replays:
            break

        added = 0
        for replay in page_replays:
            key = (replay.target.game, replay.target.token, replay.target.port)
            if key in seen:
                continue
            seen.add(key)
            replays.append(replay)
            added += 1
            if len(replays) >= args.count:
                break

        if added == 0 or len(page_replays) < page_limit:
            break
        offset += page_limit

    return replays


def cmd_list_replays(args: argparse.Namespace) -> int:
    replays = fetch_replay_list(args)
    payload = [
        {
            "fcade_url": f"fcade://stream/{r.target.emulator}/{r.target.game}/{r.target.token},{r.target.port}",
            "emulator": r.target.emulator,
            "game": r.target.game,
            "token": r.target.token,
            "port": r.target.port,
            "source": r.source,
        }
        for r in replays
    ]
    print(json.dumps(payload, indent=2))
    return 0


def cmd_bulk_download(args: argparse.Namespace) -> int:
    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    replays = fetch_replay_list(args)

    results: list[dict[str, Any]] = []
    for index, replay in enumerate(replays, start=1):
        target = replay.target
        replay_dir = out_dir / f"{target.game}-{_safe_token(target.token)}"
        quark_path = replay_dir / "quark.json"
        result: dict[str, Any] = {
            "index": index,
            "out_dir": str(replay_dir),
            "quark_json": str(quark_path),
            "emulator": target.emulator,
            "game": target.game,
            "token": target.token,
            "port": target.port,
            "source": replay.source,
        }

        summary_path = replay_dir / "summary.json"
        if args.dry_run:
            result["status"] = "dry-run"
        elif summary_path.exists() and not args.overwrite:
            replay_dir.mkdir(parents=True, exist_ok=True)
            quark_path.write_text(json.dumps(replay.source, indent=2), encoding="utf-8")
            result["status"] = "skipped-existing"
        else:
            print(f"[{index}/{len(replays)}] downloading {target.game} {target.token}", file=sys.stderr)
            try:
                replay_dir.mkdir(parents=True, exist_ok=True)
                quark_path.write_text(json.dumps(replay.source, indent=2), encoding="utf-8")
                summary = download_replay(
                    target=target,
                    host=args.host,
                    out_dir=replay_dir,
                    timeout=args.timeout,
                    idle_timeout=args.idle_timeout,
                    max_idle_timeouts=args.max_idle_timeouts,
                    max_frames=args.max_frames,
                    local_port=args.local_port,
                    send_delay_ms=args.send_delay_ms,
                )
                result["status"] = "downloaded"
                result["message_count"] = len(summary["messages"])
            except Exception as exc:  # noqa: BLE001
                result["status"] = "error"
                result["error"] = str(exc)
                if not args.keep_going:
                    results.append(result)
                    break
            if args.delay > 0 and index < len(replays):
                time.sleep(args.delay)

        results.append(result)

    manifest = {
        "created_at_unix": int(time.time()),
        "gameid": args.gameid,
        "requested_count": args.count,
        "found_count": len(replays),
        "results": results,
    }
    manifest_path = out_dir / "bulk_manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2), encoding="utf-8")
    counts: dict[str, int] = {}
    for result in results:
        status = str(result.get("status", "unknown"))
        counts[status] = counts.get(status, 0) + 1
    print(json.dumps({"out_dir": str(out_dir), "manifest": str(manifest_path), "counts": counts}, indent=2))
    return 1 if any(result.get("status") == "error" for result in results) else 0


def add_search_args(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--gameid", required=True, help="Fightcade game id, e.g. sfiii3nr1")
    parser.add_argument("--emulator", default="fbneo", help="emulator to use if API rows do not include one")
    parser.add_argument("--count", type=int, default=200, help="number of replays to fetch")
    parser.add_argument("--offset", type=int, default=0, help="initial Fightcade search offset")
    parser.add_argument("--page-size", type=int, default=15, help="Fightcade search page size")
    parser.add_argument("--best", action="store_true", help="request Fightcade best replays")
    parser.add_argument("--since", type=int, help="Fightcade since timestamp in milliseconds")
    parser.add_argument("--api-url", default=DEFAULT_API_URL)
    parser.add_argument("--api-timeout", type=float, default=20.0)
    parser.add_argument(
        "--cookie",
        help=f"optional Cookie header value, e.g. cf_clearance=...; defaults to ${DEFAULT_COOKIE_ENV}",
    )
    parser.add_argument("--user-agent", default=DEFAULT_USER_AGENT)


def add_download_stream_args(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--host", default=DEFAULT_HOST)
    parser.add_argument("--timeout", type=float, default=10.0)
    parser.add_argument(
        "--local-port",
        type=int,
        default=6004,
        help="bind local source TCP port (Fightcade client uses 6004); falls back to ephemeral if unavailable",
    )
    parser.add_argument("--idle-timeout", type=float, default=2.0)
    parser.add_argument(
        "--send-delay-ms",
        type=float,
        default=15.0,
        help="delay between handshake frames to better mimic original client pacing",
    )
    parser.add_argument(
        "--max-idle-timeouts",
        type=int,
        default=10,
        help="stop after this many consecutive idle read timeouts",
    )
    parser.add_argument("--max-frames", type=int, default=2000)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Fightcade replay downloader/parser")
    sub = parser.add_subparsers(dest="command", required=True)

    p_download = sub.add_parser("download", help="connect to GGPO replay stream and save parsed artifacts")
    p_download.add_argument("--fcade-url", help="fcade://stream/... URL")
    p_download.add_argument("--emulator", default="fbneo")
    p_download.add_argument("--game")
    p_download.add_argument("--token")
    p_download.add_argument("--port", type=int, default=DEFAULT_PORT)
    add_download_stream_args(p_download)
    p_download.add_argument("--out-dir", default="tools/fcade-replays/output")
    p_download.add_argument("--auto-dir", action="store_true", help="append <game>-<token> subdir")
    p_download.set_defaults(func=cmd_download)

    p_list = sub.add_parser("list-replays", help="list replay stream targets from Fightcade searchquarks")
    add_search_args(p_list)
    p_list.set_defaults(func=cmd_list_replays)

    p_bulk = sub.add_parser("bulk-download", help="download replay streams found via Fightcade searchquarks")
    add_search_args(p_bulk)
    add_download_stream_args(p_bulk)
    p_bulk.add_argument("--out-dir", default="tools/fcade-replays/output/bulk")
    p_bulk.add_argument("--delay", type=float, default=0.25, help="seconds to sleep between downloads")
    p_bulk.add_argument("--overwrite", action="store_true", help="download even when summary.json already exists")
    p_bulk.add_argument("--keep-going", action="store_true", help="continue after an individual replay fails")
    p_bulk.add_argument("--dry-run", action="store_true", help="write a manifest without connecting to replay streams")
    p_bulk.set_defaults(func=cmd_bulk_download)

    return parser


def main(argv: list[str]) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    try:
        return args.func(args)
    except Exception as exc:  # noqa: BLE001
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
