#!/usr/bin/env bash
# Compare a branch against a baseline by replaying identical generated inputs
# into both builds and diffing their per-frame rollback-state hashes.
#
#   tools/replay_verify.sh <baseline-ref> [seeds] [frames]
#
# Needs no ROM. Builds both sides Debug, because --stress lives behind
# NETPLAY_ENABLED and only the Debug configuration defines it.
set -euo pipefail

BASELINE="${1:-origin/main}"
SEEDS="${2:-12}"
FRAMES="${3:-1800}"

REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORK="${TMPDIR:-/tmp}/3sx-replay-verify"
BASE_TREE="$WORK/baseline"

# The repo's python tools use `X | None` annotations, so 3.10+ is required.
PY="$(command -v python3.13 || command -v python3.12 || command -v python3.11 || command -v python3.10 || command -v python3)"
if ! "$PY" -c 'import sys; sys.exit(0 if sys.version_info >= (3, 10) else 1)'; then
    echo "error: need python 3.10+, found $("$PY" --version)" >&2
    exit 1
fi

echo "==> building candidate (Debug)"
cmake -S "$REPO" -B "$REPO/build-dbg" -DCMAKE_BUILD_TYPE=Debug >/dev/null
cmake --build "$REPO/build-dbg" -j"$(sysctl -n hw.ncpu 2>/dev/null || nproc)" >/dev/null

echo "==> preparing baseline worktree at $BASELINE"
rm -rf "$BASE_TREE"
mkdir -p "$WORK"
git -C "$REPO" worktree prune
git -C "$REPO" worktree add --detach "$BASE_TREE" "$BASELINE" >/dev/null
# third_party holds prebuilt dependencies and is untracked, so a fresh
# worktree has none. Share the ones already built in the main checkout.
rm -rf "$BASE_TREE/third_party"
ln -s "$REPO/third_party" "$BASE_TREE/third_party"

echo "==> building baseline (Debug)"
cmake -S "$BASE_TREE" -B "$BASE_TREE/build-dbg" -DCMAKE_BUILD_TYPE=Debug >/dev/null
cmake --build "$BASE_TREE/build-dbg" -j"$(sysctl -n hw.ncpu 2>/dev/null || nproc)" >/dev/null

exe() { find "$1" -name 3SX -type f -perm -u+x | head -1; }

echo "==> comparing $SEEDS seeds x $FRAMES frames"
"$PY" "$REPO/tools/compare_stress_replays.py" \
    "$(exe "$BASE_TREE/build-dbg")" "$(exe "$REPO/build-dbg")" \
    --seed 1 --seeds "$SEEDS" --frames "$FRAMES"
