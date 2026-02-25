#!/usr/bin/env python3
"""Fightcade replay downloader/parser (work-in-progress reverse engineering helper)."""

from __future__ import annotations

import argparse
import json
import socket
import struct
import sys
import time
import urllib.parse
import zlib
from dataclasses import dataclass
from pathlib import Path


DEFAULT_HOST = "ggpo.fightcade.com"
DEFAULT_PORT = 7100


@dataclass
class ReplayTarget:
    emulator: str
    game: str
    token: str
    port: int


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

    messages: list[dict] = []
    count = 0

    used_local_port = local_port
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(timeout)
    if local_port > 0:
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind(("", local_port))
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

        with frames_bin.open("wb") as fw:
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
                    (out_dir / f"msg_{count:04d}_minus12.bin").write_bytes(chunk)
                    try:
                        raw = zlib.decompress(chunk)
                        (out_dir / f"msg_{count:04d}_minus12.decompressed.bin").write_bytes(raw)
                    except Exception:
                        pass
                elif msg_type == -13:
                    (out_dir / f"msg_{count:04d}_minus13.bin").write_bytes(payload[12:])

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


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Fightcade replay downloader/parser")
    sub = parser.add_subparsers(dest="command", required=True)

    p_download = sub.add_parser("download", help="connect to GGPO replay stream and save parsed artifacts")
    p_download.add_argument("--fcade-url", help="fcade://stream/... URL")
    p_download.add_argument("--emulator", default="fbneo")
    p_download.add_argument("--game")
    p_download.add_argument("--token")
    p_download.add_argument("--port", type=int, default=DEFAULT_PORT)
    p_download.add_argument("--host", default=DEFAULT_HOST)
    p_download.add_argument("--timeout", type=float, default=10.0)
    p_download.add_argument(
        "--local-port",
        type=int,
        default=6004,
        help="bind local source TCP port (Fightcade client uses 6004)",
    )
    p_download.add_argument("--idle-timeout", type=float, default=2.0)
    p_download.add_argument(
        "--send-delay-ms",
        type=float,
        default=15.0,
        help="delay between handshake frames to better mimic original client pacing",
    )
    p_download.add_argument(
        "--max-idle-timeouts",
        type=int,
        default=10,
        help="stop after this many consecutive idle read timeouts",
    )
    p_download.add_argument("--max-frames", type=int, default=2000)
    p_download.add_argument("--out-dir", default="fcade-replays/output")
    p_download.add_argument("--auto-dir", action="store_true", help="append <game>-<token> subdir")
    p_download.set_defaults(func=cmd_download)

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
