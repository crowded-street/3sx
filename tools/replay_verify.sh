#!/usr/bin/env bash
# Compare a branch against a baseline by replaying identical generated inputs
# into both builds and diffing their per-frame rollback-state hashes.
#
#   tools/replay_verify.sh <baseline-ref> [seeds] [frames]
#
# The defaults are the per-commit gate: 8 seeds x 1200 frames, about 35s once
# the baseline build is cached. Seeds run in parallel, so widening the seed
# count is much cheaper than lengthening the runs - prefer more seeds.
# Before opening a PR, run the wide gate: tools/replay_verify.sh origin/main 30 3600
#
# Needs no ROM. Builds both sides Debug, because --stress lives behind
# NETPLAY_ENABLED and only the Debug configuration defines it.
set -euo pipefail

BASELINE="${1:-origin/main}"
SEEDS="${2:-8}"
FRAMES="${3:-1200}"

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

# Reuse the baseline tree when it is already at the wanted commit. A fresh
# Debug build takes minutes; reusing makes repeat runs cost only the replays.
WANT="$(git -C "$REPO" rev-parse "$BASELINE")"
HAVE="$(git -C "$BASE_TREE" rev-parse HEAD 2>/dev/null || true)"

if [ "$WANT" != "$HAVE" ]; then
    echo "==> preparing baseline worktree at $BASELINE"
    rm -rf "$BASE_TREE"
    mkdir -p "$WORK"
    git -C "$REPO" worktree prune
    git -C "$REPO" worktree add --detach "$BASE_TREE" "$BASELINE" >/dev/null
    # third_party holds prebuilt dependencies and is untracked, so a fresh
    # worktree has none. Share the ones already built in the main checkout.
    rm -rf "$BASE_TREE/third_party"
    ln -s "$REPO/third_party" "$BASE_TREE/third_party"
else
    echo "==> reusing baseline build at $BASELINE"
fi

# The state checksum this harness records has changed shape before now, so a
# build from one side of such a change cannot be compared with one from the
# other. If the baseline is not an ancestor of HEAD, it carries commits this
# tree does not, and any of them can move the trace for reasons that have
# nothing to do with the change under test.
if ! git -C "$REPO" merge-base --is-ancestor "$BASELINE" HEAD 2>/dev/null; then
    AHEAD="$(git -C "$REPO" rev-list --count HEAD.."$BASELINE" 2>/dev/null || echo '?')"
    echo "!!  WARNING: $BASELINE is NOT an ancestor of HEAD - it has $AHEAD commit(s) this tree lacks."
    echo "!!  Divergences below may come from those, not from your change."
    echo "!!  Use the branch point instead:  tools/replay_verify.sh \$(git merge-base $BASELINE HEAD)"
fi

echo "==> building baseline (Debug)"
cmake -S "$BASE_TREE" -B "$BASE_TREE/build-dbg" -DCMAKE_BUILD_TYPE=Debug >/dev/null
cmake --build "$BASE_TREE/build-dbg" -j"$(sysctl -n hw.ncpu 2>/dev/null || nproc)" >/dev/null

exe() { find "$1" -name 3SX -type f -perm -u+x | head -1; }

echo "==> comparing $SEEDS seeds x $FRAMES frames"
set +e
"$PY" "$REPO/tools/compare_stress_replays.py" \
    "$(exe "$BASE_TREE/build-dbg")" "$(exe "$REPO/build-dbg")" \
    --seed 1 --seeds "$SEEDS" --frames "$FRAMES"
STATUS=$?
set -e

# The verdict goes last and says which it is in one line, so that reading only
# the tail of this output cannot leave you believing a diverging run passed.
echo
if [ "$STATUS" -eq 0 ]; then
    echo "REPLAY OK - $SEEDS seeds identical against $BASELINE"
else
    echo "REPLAY FAILED - divergence against $BASELINE (see the seed lines above)"
fi
exit "$STATUS"
