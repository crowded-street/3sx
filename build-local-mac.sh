#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"

if ! command -v cmake >/dev/null 2>&1; then
    echo "cmake is required but was not found on PATH."
    echo "Install it with Homebrew: brew install cmake"
    exit 1
fi

if ! command -v cpack >/dev/null 2>&1; then
    echo "cpack is required but was not found on PATH."
    echo "Install CMake with Homebrew: brew install cmake"
    exit 1
fi

echo "Building local branch on macOS"

echo "==> bootstrap dependencies"
"$ROOT_DIR/build-deps.sh"

echo "==> configure build"
cmake -S "$ROOT_DIR" -B "$ROOT_DIR/build" -DCMAKE_BUILD_TYPE=Release

echo "==> compile project"
cmake --build "$ROOT_DIR/build" --parallel

echo "==> package macOS DMG"
cpack -G DragNDrop --config "$ROOT_DIR/build/CPackConfig.cmake"

echo "==> local build complete"
