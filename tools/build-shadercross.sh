#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
THIRD_PARTY="$ROOT_DIR/third_party"

SDL_BUILD="$THIRD_PARTY/sdl3/build"
SDL_SHADERCROSS_REF="${SDL_SHADERCROSS_REF:-main}"
SDL_SHADERCROSS_DIR="$THIRD_PARTY/SDL_shadercross"
SDL_SHADERCROSS_BUILD="$SDL_SHADERCROSS_DIR/build"

mkdir -p "$THIRD_PARTY"

OS="$(uname -s)"
echo "Detected OS: $OS"

echo "Using cmake from: $(which cmake)"
cmake --version

detect_build_jobs() {
    case "$OS" in
        Darwin)
            sysctl -n hw.ncpu
            ;;
        MINGW*|MSYS*|CYGWIN*)
            echo "${NUMBER_OF_PROCESSORS:-4}"
            ;;
        *)
            if command -v nproc >/dev/null 2>&1; then
                nproc
            elif command -v getconf >/dev/null 2>&1; then
                getconf _NPROCESSORS_ONLN
            else
                echo 4
            fi
            ;;
    esac
}

BUILD_JOBS="${BUILD_JOBS:-}"
if [ -z "$BUILD_JOBS" ]; then
    BUILD_JOBS="$(detect_build_jobs)"
fi

echo "Using $BUILD_JOBS build job(s)"

if [ ! -d "$SDL_BUILD/lib/cmake/SDL3" ]; then
    echo "SDL3 build not found at $SDL_BUILD"
    echo "Run ./build-deps.sh first so SDL_shadercross can link against SDL3."
    exit 1
fi

if [ -d "$SDL_SHADERCROSS_BUILD" ]; then
    echo "SDL_shadercross already built at $SDL_SHADERCROSS_BUILD"
    exit 0
fi

echo "Building SDL_shadercross @ $SDL_SHADERCROSS_REF..."

SDL_SHADERCROSS_SRC=$(mktemp -d)
trap 'rm -rf "$SDL_SHADERCROSS_SRC"' EXIT

git clone \
    --branch "$SDL_SHADERCROSS_REF" \
    --single-branch \
    --recurse-submodules \
    https://github.com/libsdl-org/SDL_shadercross.git \
    "$SDL_SHADERCROSS_SRC"

cmake -S "$SDL_SHADERCROSS_SRC" -B "$SDL_SHADERCROSS_SRC/cmake-build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$SDL_SHADERCROSS_BUILD" \
    -DCMAKE_PREFIX_PATH="$SDL_BUILD" \
    -DSDL3_DIR="$SDL_BUILD/lib/cmake/SDL3" \
    -DBUILD_SHARED_LIBS=OFF \
    -DSDLSHADERCROSS_VENDORED=ON \
    -DSDLSHADERCROSS_SPIRVCROSS_SHARED=OFF \
    -DSDLSHADERCROSS_SHARED=OFF \
    -DSDLSHADERCROSS_STATIC=ON \
    -DSDLSHADERCROSS_CLI=ON \
    -DSDLSHADERCROSS_CLI_STATIC=ON \
    -DSDLSHADERCROSS_INSTALL=ON

cmake --build "$SDL_SHADERCROSS_SRC/cmake-build" --target shadercross -j"$BUILD_JOBS"
cmake --install "$SDL_SHADERCROSS_SRC/cmake-build"

if [ "$OS" = "Darwin" ] && [ -f "$SDL_SHADERCROSS_BUILD/bin/shadercross" ]; then
    install_name_tool -add_rpath "@executable_path/../lib" "$SDL_SHADERCROSS_BUILD/bin/shadercross" || true
fi

echo "SDL_shadercross CLI installed to $SDL_SHADERCROSS_BUILD"
