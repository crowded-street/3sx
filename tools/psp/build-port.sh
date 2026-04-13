#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="${1:-build/psp}"
BUILD_TYPE="${CMAKE_BUILD_TYPE:-Release}"
shift $(( $# > 0 ? 1 : 0 ))
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

toolchain_file="$(find "${PSPDEV:-/usr/local/pspdev}" -name pspdev.cmake 2>/dev/null | head -n 1)"
if [ -z "${toolchain_file}" ]; then
  echo 'Could not find pspdev.cmake. Run this inside the PSP container with PSPDEV installed.' >&2
  exit 1
fi

cmake -S "${REPO_ROOT}" -B "${REPO_ROOT}/${BUILD_DIR}" \
  -DCMAKE_TOOLCHAIN_FILE="${toolchain_file}" \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  "$@"

cmake --build "${REPO_ROOT}/${BUILD_DIR}" -j"$(nproc)"
