#!/usr/bin/env python3
import argparse
import re
import struct
import sys
from pathlib import Path


MAGIC = b"SCRD"
HEADER_SIZE = 6
TABLE_ENTRY_SIZE = 8
MAX_FRAMES = 0xFFFF
MAX_U32 = 0xFFFFFFFF
FRAME_NAME_PREFIX = "frame_"
FRAME_NAME_SUFFIX = ".ram"
MAX_RUN_SIZE = 128
ZERO_RUN_FLAG = 0x80
ZERO_RUN_PATTERN = re.compile(b"\0{2,}")


def zero_run_encode(data: bytes) -> bytes:
    encoded = bytearray()
    literal_start = 0

    def append_literal(start: int, end: int) -> None:
        while start < end:
            size = min(end - start, MAX_RUN_SIZE)
            encoded.append(size - 1)
            encoded.extend(data[start : start + size])
            start += size

    for match in ZERO_RUN_PATTERN.finditer(data):
        append_literal(literal_start, match.start())
        zeroes_left = match.end() - match.start()

        while zeroes_left:
            size = min(zeroes_left, MAX_RUN_SIZE)
            encoded.append(ZERO_RUN_FLAG | (size - 1))
            zeroes_left -= size

        literal_start = match.end()

    append_literal(literal_start, len(data))
    return bytes(encoded)


def parse_frame_number(path: Path) -> int | None:
    name = path.name

    if not name.startswith(FRAME_NAME_PREFIX) or not name.endswith(FRAME_NAME_SUFFIX):
        return None

    digits = name[len(FRAME_NAME_PREFIX) : -len(FRAME_NAME_SUFFIX)]

    if len(digits) != 8 or not digits.isdigit():
        return None

    return int(digits, 10)


def find_frame_paths(input_dir: Path) -> list[Path]:
    numbered_paths: list[tuple[int, Path]] = []

    for path in input_dir.iterdir():
        if not path.is_file():
            continue

        frame_number = parse_frame_number(path)

        if frame_number is not None:
            numbered_paths.append((frame_number, path))

    if not numbered_paths:
        raise RuntimeError(f"no RAM dump frames found in {input_dir}")

    numbered_paths.sort(key=lambda item: item[0])

    if numbered_paths[0][0] != 0:
        raise RuntimeError(f"first frame must be frame_00000000.ram, got {numbered_paths[0][1].name}")

    for expected, (frame_number, path) in enumerate(numbered_paths):
        if frame_number != expected:
            raise RuntimeError(f"missing frame_{expected:08d}.ram before {path.name}")

    if len(numbered_paths) > MAX_FRAMES:
        raise RuntimeError(f"too many frames: {len(numbered_paths)} exceeds the u16 limit of {MAX_FRAMES}")

    return [path for _, path in numbered_paths]


def xor_bytes(left: bytes, right: bytes) -> bytes:
    if len(left) != len(right):
        raise RuntimeError(f"cannot XOR frames with different sizes: {len(left)} and {len(right)} bytes")

    return bytes(a ^ b for a, b in zip(left, right))


def compress_ram_dumps(input_dir: Path, output_path: Path, force: bool) -> int:
    if not input_dir.is_dir():
        raise RuntimeError(f"input path is not a directory: {input_dir}")

    if output_path.exists() and not force:
        raise RuntimeError(f"output file already exists: {output_path} (use --force to overwrite)")

    frame_paths = find_frame_paths(input_dir)
    table_offset = HEADER_SIZE
    data_offset = HEADER_SIZE + len(frame_paths) * TABLE_ENTRY_SIZE
    frame_table: list[tuple[int, int]] = []
    previous_frame: bytes | None = None
    expected_frame_size: int | None = None

    output_path.parent.mkdir(parents=True, exist_ok=True)

    with output_path.open("wb") as out_file:
        out_file.write(MAGIC)
        out_file.write(struct.pack("<H", len(frame_paths)))
        out_file.write(bytes(len(frame_paths) * TABLE_ENTRY_SIZE))

        for frame_index, frame_path in enumerate(frame_paths):
            frame = frame_path.read_bytes()

            if expected_frame_size is None:
                expected_frame_size = len(frame)
            elif len(frame) != expected_frame_size:
                raise RuntimeError(
                    f"{frame_path.name} is {len(frame)} bytes, expected {expected_frame_size} bytes"
                )

            payload = frame if previous_frame is None else xor_bytes(frame, previous_frame)
            compressed = zero_run_encode(payload)
            offset = out_file.tell()
            size = len(compressed)

            if offset > MAX_U32:
                raise RuntimeError(f"frame {frame_index} offset exceeds u32 limit")

            if size > MAX_U32:
                raise RuntimeError(f"frame {frame_index} compressed size exceeds u32 limit")

            out_file.write(compressed)
            frame_table.append((offset, size))
            previous_frame = frame

        out_file.seek(table_offset)

        for offset, size in frame_table:
            out_file.write(struct.pack("<II", offset, size))

    final_size = output_path.stat().st_size

    if final_size < data_offset:
        raise RuntimeError("internal error: output ended before frame data")

    return len(frame_paths)


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Package frame_XXXXXXXX.ram files into a zero-run-encoded SCRD archive."
    )
    parser.add_argument("input_dir", type=Path, help="directory containing frame_00000000.ram, etc.")
    parser.add_argument("output_path", type=Path, help="SCRD archive to write")
    parser.add_argument("-f", "--force", action="store_true", help="overwrite output_path if it exists")
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = parse_args(argv)

    try:
        frame_count = compress_ram_dumps(args.input_dir, args.output_path, args.force)
    except RuntimeError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    print(f"wrote {args.output_path} ({frame_count} frames)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
