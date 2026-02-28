#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
THIRD_PARTY="$ROOT_DIR/third_party"

mkdir -p "$THIRD_PARTY"

# Detect OS
OS="$(uname -s)"
echo "Detected OS: $OS"

echo "Using cmake from: $(which cmake)"
cmake --version

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
                --extra-cflags="-I/mingw64/include" \
                --extra-ldflags="-L/mingw64/lib"
            ;;
        *)
            echo "Unsupported OS: $OS"
            exit 1
            ;;
    esac

    make -j$(nproc)
    make install
    echo "FFmpeg installed to $FFMPEG_BUILD"

    cd ../..
    rm -rf "$FFMPEG"
    rm "$FFMPEG.tar.xz"
fi

# -----------------------------
# SDL3
# -----------------------------

SDL="SDL3-3.4.0"
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
                -DCMAKE_INSTALL_PREFIX="$SDL_BUILD" \
                -DBUILD_SHARED_LIBS=ON
            ;;
    esac

    cmake --build . -j$(nproc)
    cmake --install .
    echo "SDL3 installed to $SDL_BUILD"

    cd ../..
    rm -rf "$SDL"
    rm "$SDL.tar.gz"
fi

# -----------------------------
# GekkoNet
# -----------------------------

GEKKONET_REF="7be848c"
GEKKONET_DIR="$THIRD_PARTY/GekkoNet"
GEKKONET_BUILD="$GEKKONET_DIR/build"

if [ -d "$GEKKONET_BUILD" ]; then
    echo "GekkoNet already built at $GEKKONET_BUILD"
else
    echo "Building GekkoNet @ $GEKKONET_REF..."

    GEKKONET_SRC=$(mktemp -d)
    git clone https://github.com/HeatXD/GekkoNet.git "$GEKKONET_SRC"
    git -C "$GEKKONET_SRC" -c advice.detachedHead=false checkout "$GEKKONET_REF"

    cmake -S "$GEKKONET_SRC" -B "$GEKKONET_SRC/cmake-build" \
        -DCMAKE_BUILD_TYPE=Release \
        -DNO_ASIO_BUILD=ON \
        -DBUILD_SHARED_LIBS=OFF

    cmake --build "$GEKKONET_SRC/cmake-build" -j$(nproc)

    mkdir -p "$GEKKONET_BUILD/include" "$GEKKONET_BUILD/lib"
    cp -r "$GEKKONET_SRC/GekkoLib/include/." "$GEKKONET_BUILD/include/"
    find "$GEKKONET_SRC" -name "*.a" -exec cp {} "$GEKKONET_BUILD/lib/libGekkoNet.a" \;

    rm -rf "$GEKKONET_SRC"
    echo "GekkoNet installed to $GEKKONET_BUILD"
fi

# -----------------------------
# SDL3_net
# -----------------------------

SDL3_NET_REF="92022dc"
SDL3_NET_DIR="$THIRD_PARTY/SDL_net"
SDL3_NET_BUILD="$SDL3_NET_DIR/build"

if [ -d "$SDL3_NET_BUILD" ]; then
    echo "SDL3_net already built at $SDL3_NET_BUILD"
else
    echo "Building SDL3_net @ $SDL3_NET_REF..."

    SDL3_NET_SRC=$(mktemp -d)
    git clone https://github.com/libsdl-org/SDL_net.git "$SDL3_NET_SRC"
    git -C "$SDL3_NET_SRC" -c advice.detachedHead=false checkout "$SDL3_NET_REF"

    cmake -S "$SDL3_NET_SRC" -B "$SDL3_NET_SRC/cmake-build" \
        -DCMAKE_INSTALL_PREFIX="$SDL3_NET_BUILD" \
        -DCMAKE_PREFIX_PATH="$SDL_BUILD" \
        -DBUILD_SHARED_LIBS=OFF \
        -DSDLNET_INSTALL=ON

    cmake --build "$SDL3_NET_SRC/cmake-build" -j$(nproc)
    cmake --install "$SDL3_NET_SRC/cmake-build"

    rm -rf "$SDL3_NET_SRC"
    echo "SDL3_net installed to $SDL3_NET_BUILD"
fi

# -----------------------------
# libcdio
# -----------------------------

LIBCDIO_VERSION="2.3.0"
LIBCDIO="libcdio-$LIBCDIO_VERSION"
LIBCDIO_DIR="$THIRD_PARTY/libcdio"
LIBCDIO_BUILD="$LIBCDIO_DIR/build"

if [ -d "$LIBCDIO_DIR" ]; then
    echo "libcdio already built at $LIBCDIO_BUILD"
else
    echo "Building libcdio..."
    mkdir -p "$LIBCDIO_DIR"
    cd "$LIBCDIO_DIR"

    if [ ! -d "$LIBCDIO" ]; then
        curl -L -O "https://github.com/libcdio/libcdio/releases/download/$LIBCDIO_VERSION/$LIBCDIO.tar.gz"
        tar xf "$LIBCDIO.tar.gz"
    fi

    cd "$LIBCDIO"

    mkdir -p build
    cd build

    sh ../configure MAKE=make \
        --prefix=$LIBCDIO_BUILD \
        --enable-static \
        --disable-shared \
        --disable-cxx \
        --disable-example-progs

    make
    make install
    echo "libcdio installed to $LIBCDIO_BUILD"

    cd ../..
    rm -rf "$LIBCDIO"
    rm "$LIBCDIO.tar.gz"
fi

echo "All dependencies installed successfully in $THIRD_PARTY"
