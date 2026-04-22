#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
THIRD_PARTY="${ROOT_DIR}/third_party/armhf"
TRIPLE="${CRS_ARM_TRIPLE:-arm-linux-gnueabihf}"
JOBS="${JOBS:-$(nproc 2>/dev/null || echo 2)}"
CPU_FLAGS="${CRS_ARM_CPU_FLAGS:--march=armv7-a -mfpu=neon -mfloat-abi=hard}"
SYSROOT="${CRS_ARM_SYSROOT:-}"
TOOLCHAIN_FILE="${ROOT_DIR}/cmake/toolchain-armhf.cmake"

export CC="${TRIPLE}-gcc"
export CXX="${TRIPLE}-g++"
export AR="${TRIPLE}-ar"
export RANLIB="${TRIPLE}-ranlib"
export STRIP="${TRIPLE}-strip"
export PKG_CONFIG_PATH=""

if [ -n "${SYSROOT}" ] && [ -f "${SYSROOT}/lib/arm-linux-gnueabihf/libc.so.6" ]; then
    CPU_FLAGS="${CPU_FLAGS} --sysroot=${SYSROOT}"
fi

mkdir -p "${THIRD_PARTY}"

ZLIB_TAG="v1.3.1"
SDL_TAG="release-3.4.4"
FFMPEG_VERSION="ffmpeg-8.0"
LIBCDIO_VERSION="2.3.0"
MINIZIP_NG_TAG="4.1.0"
TF_PSA_CRYPTO_VERSION="1.0.0"

build_zlib() {
    local out_dir="${THIRD_PARTY}/zlib/build"
    [ -f "${out_dir}/lib/libz.a" ] && return 0
    local src
    src="$(mktemp -d)"
    curl -fsSL -o "${src}/zlib.tar.gz" "https://github.com/madler/zlib/archive/refs/tags/${ZLIB_TAG}.tar.gz"
    tar xf "${src}/zlib.tar.gz" -C "${src}"
    (
        cd "${src}/zlib-${ZLIB_TAG#v}"
        CHOST="${TRIPLE}" CFLAGS="${CPU_FLAGS} -fPIC" ./configure --prefix="${out_dir}" --static
        make -j"${JOBS}"
        make install
    )
    rm -rf "${src}"
}

build_libcdio() {
    local out_dir="${THIRD_PARTY}/libcdio/build"
    [ -f "${out_dir}/lib/libcdio.a" ] && return 0
    local src
    src="$(mktemp -d)"
    curl -fsSL -o "${src}/libcdio.tar.gz" \
        "https://github.com/libcdio/libcdio/releases/download/${LIBCDIO_VERSION}/libcdio-${LIBCDIO_VERSION}.tar.gz"
    tar xf "${src}/libcdio.tar.gz" -C "${src}"
    (
        cd "${src}/libcdio-${LIBCDIO_VERSION}"
        CFLAGS="${CPU_FLAGS}" ./configure MAKE=make \
            --host="${TRIPLE}" \
            --prefix="${out_dir}" \
            --enable-static \
            --disable-shared \
            --disable-cxx \
            --disable-example-progs
        make -j"${JOBS}"
        make install
    )
    rm -rf "${src}"
}

build_minizip_ng() {
    local out_dir="${THIRD_PARTY}/minizip-ng/build"
    [ -f "${out_dir}/lib/libminizip-ng.a" ] && return 0
    local src
    src="$(mktemp -d)"
    git clone --depth=1 --branch "${MINIZIP_NG_TAG}" https://github.com/zlib-ng/minizip-ng "${src}/minizip-ng"
    cmake -S "${src}/minizip-ng" -B "${src}/build" \
        -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" \
        -DCMAKE_INSTALL_PREFIX="${out_dir}" \
        -DCRS_ARM_TRIPLE="${TRIPLE}" \
        -DCRS_ARM_CPU_FLAGS="${CPU_FLAGS}" \
        -DCRS_ARM_SYSROOT="${SYSROOT}" \
        -DZLIB_LIBRARY="${THIRD_PARTY}/zlib/build/lib/libz.a" \
        -DZLIB_INCLUDE_DIR="${THIRD_PARTY}/zlib/build/include" \
        -DMZ_COMPAT=OFF -DMZ_ZLIB_FLAVOR=zlib \
        -DMZ_BZIP2=OFF -DMZ_LZMA=OFF -DMZ_PPMD=OFF -DMZ_ZSTD=OFF -DMZ_LIBCOMP=OFF \
        -DMZ_PKCRYPT=OFF -DMZ_WZAES=OFF -DMZ_OPENSSL=OFF -DMZ_LIBBSD=OFF \
        -DMZ_DECOMPRESS_ONLY=ON
    cmake --build "${src}/build" -j"${JOBS}"
    cmake --install "${src}/build"
    rm -rf "${src}"
}

build_tf_psa_crypto() {
    local out_dir="${THIRD_PARTY}/tf-psa-crypto/build"
    [ -f "${out_dir}/lib/libtfpsacrypto.a" ] && return 0
    local src
    src="$(mktemp -d)"
    curl -fsSL -o "${src}/tfpsa.tar.bz2" \
        "https://github.com/Mbed-TLS/TF-PSA-Crypto/releases/download/tf-psa-crypto-${TF_PSA_CRYPTO_VERSION}/tf-psa-crypto-${TF_PSA_CRYPTO_VERSION}.tar.bz2"
    tar xf "${src}/tfpsa.tar.bz2" -C "${src}"
    cmake -S "${src}/tf-psa-crypto-${TF_PSA_CRYPTO_VERSION}" -B "${src}/build" \
        -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="${out_dir}" \
        -DCRS_ARM_TRIPLE="${TRIPLE}" \
        -DCRS_ARM_CPU_FLAGS="${CPU_FLAGS}" \
        -DCRS_ARM_SYSROOT="${SYSROOT}" \
        -DCMAKE_C_FLAGS="${CPU_FLAGS} -Wno-error=array-bounds -Wno-error=stringop-overflow" \
        -DENABLE_PROGRAMS=OFF -DENABLE_TESTING=OFF \
        -DUSE_SHARED_TF_PSA_CRYPTO_LIBRARY=OFF \
        -DUSE_STATIC_TF_PSA_CRYPTO_LIBRARY=ON \
        -DTF_PSA_CRYPTO_CONFIG_FILE="configs/crypto-config-ccm-aes-sha256.h"
    cmake --build "${src}/build" -j"${JOBS}"
    cmake --install "${src}/build"
    rm -rf "${src}"
}

build_sdl3() {
    local out_dir="${THIRD_PARTY}/sdl3/build"
    [ -f "${out_dir}/lib/libSDL3.so" ] && return 0
    local src
    src="$(mktemp -d)"
    git clone --depth=1 --branch "${SDL_TAG}" https://github.com/libsdl-org/SDL "${src}/sdl3"
    cmake -S "${src}/sdl3" -B "${src}/build" \
        -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" \
        -DCMAKE_INSTALL_PREFIX="${out_dir}" \
        -DCRS_ARM_TRIPLE="${TRIPLE}" \
        -DCRS_ARM_CPU_FLAGS="${CPU_FLAGS}" \
        -DCRS_ARM_SYSROOT="${SYSROOT}" \
        -DCMAKE_EXE_LINKER_FLAGS="-lpthread -lrt -ldl" \
        -DCMAKE_SHARED_LINKER_FLAGS="-lpthread -lrt -ldl" \
        -DBUILD_SHARED_LIBS=ON \
        -DSDL_STATIC=OFF \
        -DSDL_TEST_LIBRARY=OFF \
        -DSDL_TESTS=OFF \
        -DSDL_INSTALL_TESTS=OFF \
        -DSDL_UNIX_CONSOLE_BUILD=ON \
        -DSDL_WAYLAND=OFF \
        -DSDL_X11=OFF \
        -DSDL_KMSDRM=OFF \
        -DSDL_OPENGL=OFF \
        -DSDL_OPENGLES=OFF \
        -DSDL_VULKAN=OFF \
        -DSDL_ALSA=ON \
        -DSDL_PULSEAUDIO=OFF \
        -DSDL_PIPEWIRE=OFF \
        -DSDL_JACK=OFF
    cmake --build "${src}/build" -j"${JOBS}"
    cmake --install "${src}/build"
    rm -rf "${src}"
}

build_ffmpeg() {
    local out_dir="${THIRD_PARTY}/ffmpeg/build"
    [ -f "${out_dir}/lib/libavcodec.so" ] && return 0
    local src
    local extra_ldflags="-Wl,-rpath,\$\$ORIGIN/../lib"
    local sysroot_arg=()
    src="$(mktemp -d)"

    if [ -n "${SYSROOT}" ] && [ -f "${SYSROOT}/lib/arm-linux-gnueabihf/libc.so.6" ]; then
        sysroot_arg=(--sysroot="${SYSROOT}")
        extra_ldflags="--sysroot=${SYSROOT} ${extra_ldflags}"
    fi

    curl -fsSL -o "${src}/ffmpeg.tar.xz" "https://ffmpeg.org/releases/${FFMPEG_VERSION}.tar.xz"
    tar xf "${src}/ffmpeg.tar.xz" -C "${src}"
    (
        cd "${src}/${FFMPEG_VERSION}"
        mkdir -p build && cd build
        ../configure \
            --prefix="${out_dir}" \
            --enable-cross-compile \
            --cross-prefix="${TRIPLE}-" \
            --arch=arm \
            --target-os=linux \
            "${sysroot_arg[@]}" \
            --disable-all --disable-autodetect \
            --disable-static --enable-shared \
            --enable-avcodec --enable-avformat --enable-avutil --enable-swresample \
            --enable-decoder=adpcm_adx --enable-parser=adx --enable-muxer=adx \
            --enable-pic \
            --extra-cflags="${CPU_FLAGS} -fPIC" \
            --extra-ldflags="${extra_ldflags}"
        make -j"${JOBS}"
        make install
    )
    rm -rf "${src}"
}

build_zlib
build_libcdio
build_minizip_ng
build_tf_psa_crypto
build_sdl3
build_ffmpeg
