#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
OS="$(uname -s)"

case "$OS" in
    Darwin)
        echo "Detected macOS"
        exec "$ROOT_DIR/build-local-mac.sh"
        ;;
    *)
        echo "Unsupported OS: $OS"
        echo "This local convenience build script currently supports macOS only."
        exit 1
        ;;
esac
