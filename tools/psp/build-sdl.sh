#!/usr/bin/env bash

set -euo pipefail

SDL_TAG="${SDL_TAG:-release-3.4.4}"
BUILD_TYPE="${CMAKE_BUILD_TYPE:-Release}"
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
SDL_PREFIX="${REPO_ROOT}/third_party/sdl3/build"

toolchain_file="$(find "${PSPDEV:-/usr/local/pspdev}" -name pspdev.cmake 2>/dev/null | head -n 1)"
if [ -z "${toolchain_file}" ]; then
  echo 'Could not find pspdev.cmake. Run this inside the PSP container with PSPDEV installed.' >&2
  exit 1
fi

sdl_src="$(mktemp -d)"
trap 'rm -rf "${sdl_src}"' EXIT

git clone \
  --branch "${SDL_TAG}" \
  --single-branch \
  https://github.com/libsdl-org/SDL \
  "${sdl_src}"

rm -rf "${SDL_PREFIX}"

cmake -S "${sdl_src}" -B "${sdl_src}/cmake-build" \
  -DCMAKE_TOOLCHAIN_FILE="${toolchain_file}" \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -DCMAKE_INSTALL_PREFIX="${SDL_PREFIX}" \
  -DBUILD_SHARED_LIBS=OFF \
  -DSDL_SHARED=OFF \
  -DSDL_STATIC=ON \
  -DSDL_TESTS=OFF \
  -DSDL_TEST_LIBRARY=OFF \
  -DSDL_EXAMPLES=OFF \
  -DSDL_DISABLE_INSTALL_DOCS=ON

cmake --build "${sdl_src}/cmake-build" -j"$(nproc)"
cmake --install "${sdl_src}/cmake-build"
