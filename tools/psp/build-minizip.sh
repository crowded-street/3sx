#!/usr/bin/env bash

set -euo pipefail

MINIZIP_NG_TAG="4.1.0"
BUILD_TYPE="${CMAKE_BUILD_TYPE:-Release}"
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
MINIZIP_NG_PREFIX="${REPO_ROOT}/third_party/minizip-ng/build"

toolchain_file="$(find "${PSPDEV:-/usr/local/pspdev}" -name pspdev.cmake 2>/dev/null | head -n 1)"
if [ -z "${toolchain_file}" ]; then
  echo 'Could not find pspdev.cmake. Run this inside the PSP container with PSPDEV installed.' >&2
  exit 1
fi

minizip_src="$(mktemp -d)"
trap 'rm -rf "${minizip_src}"' EXIT

git clone \
  --branch "${MINIZIP_NG_TAG}" \
  --single-branch \
  https://github.com/zlib-ng/minizip-ng \
  "${minizip_src}"

cmake -S "${minizip_src}" -B "${minizip_src}/cmake-build" \
  -DCMAKE_TOOLCHAIN_FILE="${toolchain_file}" \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -DCMAKE_INSTALL_PREFIX="${MINIZIP_NG_PREFIX}" \
  -DCMAKE_C_FLAGS=-G0 \
  -DBUILD_SHARED_LIBS=OFF \
  -DMZ_COMPAT=OFF \
  -DMZ_ZLIB_FLAVOR=zlib \
  -DMZ_BZIP2=OFF \
  -DMZ_LZMA=OFF \
  -DMZ_ZSTD=OFF \
  -DMZ_LIBCOMP=OFF \
  -DMZ_PKCRYPT=OFF \
  -DMZ_WZAES=OFF \
  -DMZ_OPENSSL=OFF \
  -DMZ_LIBBSD=OFF \
  -DMZ_ICONV=OFF \
  -DMZ_FETCH_LIBS=OFF \
  -DMZ_DECOMPRESS_ONLY=ON \
  -DMZ_FILE32_API=ON

cmake --build "${minizip_src}/cmake-build" -j"$(nproc)"
cmake --install "${minizip_src}/cmake-build"
