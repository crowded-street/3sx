#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
THIRD_PARTY="$ROOT_DIR/third_party"

mkdir -p "$THIRD_PARTY"

# Detect OS
OS="$(uname -s)"
echo "Detected OS: $OS"

# -----------------------------
# FFmpeg
# -----------------------------

FFMPEG="ffmpeg-8.0"
FFMPEG_DIR="$THIRD_PARTY/ffmpeg"
FFMPEG_BUILD="$FFMPEG_DIR/build"

if [ -d "$FFMPEG_BUILD" ]; then
    echo "FFmpeg already built at $FFMPEG_BUILD"
else
    echo "Building FFmpeg..."
    mkdir -p "$FFMPEG_DIR"
    cd "$FFMPEG_DIR"

    if [ ! -d "$FFMPEG" ]; then
        curl -L -O "https://ffmpeg.org/releases/$FFMPEG.tar.xz"
        tar xf "$FFMPEG.tar.xz"
    fi

    cd "$FFMPEG"

    mkdir -p build
    cd build

    case "$OS" in
        Darwin)
            ../configure \
                --prefix=$FFMPEG_BUILD \
                --disable-all --disable-autodetect \
                --disable-static --enable-shared \
                --enable-avcodec --enable-avformat --enable-avutil --enable-swresample \
                --enable-decoder=adpcm_adx --enable-parser=adx --enable-muxer=adx \
                --enable-pic \
                --extra-cflags="-fPIC" \
                --extra-ldflags="-Wl,-rpath,@loader_path/../Frameworks" \
                --install-name-dir="@rpath"
            ;;
        Linux)
            ../configure \
                --prefix=$FFMPEG_BUILD \
                --disable-all --disable-autodetect \
                --disable-static --enable-shared \
                --enable-avcodec --enable-avformat --enable-avutil --enable-swresample \
                --enable-decoder=adpcm_adx --enable-parser=adx --enable-muxer=adx \
                --enable-pic \
                --extra-cflags="-fPIC" \
                --extra-ldflags="-Wl,-rpath,\$ORIGIN/../lib" \
                --install-name-dir=\$ORIGIN
            ;;
        MINGW*|MSYS*|CYGWIN*)
            ../configure \
                --prefix=$FFMPEG_BUILD \
                --disable-all --disable-autodetect \
                --disable-static --enable-shared \
                --enable-avcodec --enable-avformat --enable-avutil --enable-swresample \
                --enable-decoder=adpcm_adx --enable-parser=adx --enable-muxer=adx \
                --extra-cflags="-IC:/msys64/mingw64/include" \
                --extra-ldflags="-LC:/msys64/mingw64/lib"
            ;;
        *)
            echo "Unsupported OS: $OS"
            exit 1
            ;;
    esac

    make -j$(nproc)
    make install
    echo "FFmpeg installed to $FFMPEG_BUILD"
fi

# -----------------------------
# SDL3
# -----------------------------

SDL="SDL3-3.2.24"
SDL_DIR="$THIRD_PARTY/sdl3"
SDL_BUILD="$SDL_DIR/build"

if [ -d "$SDL_BUILD" ]; then
    echo "SDL3 already built at $SDL_BUILD"
else
    echo "Building SDL3..."
    mkdir -p "$SDL_DIR"
    cd "$SDL_DIR"

    if [ ! -d "$SDL" ]; then
        curl -L -O "https://libsdl.org/release/$SDL.tar.gz"
        tar xf "$SDL.tar.gz"
    fi

    cd "$SDL"

    mkdir -p build
    cd build

    case "$OS" in
        Darwin|Linux)
            cmake .. \
                -DCMAKE_INSTALL_PREFIX="$SDL_BUILD" \
                -DBUILD_SHARED_LIBS=ON \
                -DSDL_STATIC=OFF
            ;;
        MINGW*|MSYS*|CYGWIN*)
            cmake .. \
                -G "MinGW Makefiles" \
                -DCMAKE_INSTALL_PREFIX="$SDL_BUILD" \
                -DBUILD_SHARED_LIBS=ON
            ;;
    esac

    cmake --build . -j$(nproc)
    cmake --install .
    echo "SDL3 installed to $SDL_BUILD"
fi

echo "All dependencies installed successfully in $THIRD_PARTY"
