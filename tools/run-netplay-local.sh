#!/usr/bin/env bash
set -euo pipefail

# Launches two local instances wired to each other over the direct P2P path.
# Player 1 binds UDP 50000 and talks to 50001, player 2 does the reverse:
# Netplay_SetParams picks those ports whenever the remote IP is 127.0.0.1.
#
# Pick NETWORK then DIRECT CONNECT in both windows, which is what starts the session
# and binds the sockets. Each instance logs next to the executable.

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
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

if [ ! -x "$EXE" ]; then
    echo "Could not find $EXE."
    echo "Build and install a netplay-enabled build first:"
    echo "    cmake --build build --parallel"
    echo "    cmake --install build --prefix build/application"
    exit 1
fi

start_player() {
    local player="$1"
    shift

    # Run from the bin directory so the game finds its assets and shaders.
    (cd "$BIN" && "$EXE" --p2p-local-player "$player" --p2p-remote-ip 127.0.0.1 "$@") \
        > "$BIN/netplay-p$player.log" 2>&1 &
}

echo "Starting player 1 (port 50000)..."
start_player 1

# Both instances would otherwise play the same audio twice.
echo "Starting player 2 (port 50001)..."
start_player 2 --no-sound

echo
echo "Now pick NETWORK then DIRECT CONNECT in both windows."
echo "Logs: $BIN/netplay-p1.log and netplay-p2.log"

# Keep both attached to this shell so Ctrl+C stops the pair.
wait
