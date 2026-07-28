#!/usr/bin/env bash
set -euo pipefail

# Launches two local instances wired to each other over the direct P2P path.
# Player 1 binds UDP 50000 and talks to 50001, player 2 does the reverse:
# Netplay_SetParams picks those ports whenever the remote IP is 127.0.0.1.
#
# Set THREESX_EXE to the game executable if your build does not live in
# build/application, which is what docs/building.md installs to:
#
#     THREESX_EXE=path/to/3sx tools/run-netplay-local.sh
#
# Pick NETWORK then DIRECT CONNECT in both windows, which is what starts the session
# and binds the sockets. Each instance logs next to the executable.

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

EXE="${THREESX_EXE:-}"

if [ -z "$EXE" ]; then
    BIN="$ROOT_DIR/build/application/bin"

    case "$(uname -s)" in
        Darwin)
            EXE="$BIN/3SX.app/Contents/MacOS/3SX"
            ;;
        MINGW*|MSYS*|CYGWIN*)
            EXE="$BIN/3sx.exe"
            ;;
        *)
            EXE="$BIN/3sx"
            ;;
    esac
fi

if [ ! -x "$EXE" ]; then
    echo "Could not find an executable at $EXE."
    echo "Either install a netplay-enabled build to the default location:"
    echo "    cmake --build build --parallel"
    echo "    cmake --install build --prefix build/application"
    echo "or point THREESX_EXE at the executable you already have."
    exit 1
fi

# start_player cds before running, so a relative override has to be resolved first.
EXE="$(cd "$(dirname "$EXE")" && pwd)/$(basename "$EXE")"

# Run from the directory holding the executable so the game finds its assets and
# shaders. For a macOS bundle that is the directory holding the .app instead.
case "$EXE" in
    *.app/Contents/MacOS/*)
        RUN_DIR="$(dirname "${EXE%%.app/Contents/MacOS/*}")"
        ;;
    *)
        RUN_DIR="$(dirname "$EXE")"
        ;;
esac

start_player() {
    local player="$1"
    shift

    (cd "$RUN_DIR" && "$EXE" --p2p-local-player "$player" --p2p-remote-ip 127.0.0.1 "$@") \
        > "$RUN_DIR/netplay-p$player.log" 2>&1 &
}

echo "Using $EXE"
echo "Starting player 1 (port 50000)..."
start_player 1

# Both instances would otherwise play the same audio twice.
echo "Starting player 2 (port 50001)..."
start_player 2 --no-sound

echo
echo "Now pick NETWORK then DIRECT CONNECT in both windows."
echo "Logs: $RUN_DIR/netplay-p1.log and netplay-p2.log"

# Keep both attached to this shell so Ctrl+C stops the pair.
wait
