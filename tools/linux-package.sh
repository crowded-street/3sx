#!/usr/bin/env bash
#
# package-linux.sh
#
# Builds .deb, .rpm and .pkg.tar.zst (Arch) packages for 3SX from an
# already-installed staging directory (see `cmake --install`).
#
# The binary is built with INSTALL_RPATH "$ORIGIN/../lib" (see CMakeLists.txt),
# meaning it expects its bundled libSDL3.so to sit one directory above it
# (bin/../lib). We must NOT dump bin/ and lib/ straight into the shared
# /usr/bin and /usr/lib -- that would spill our bundled SDL3.so into the
# system-wide lib directory and risk clobbering / conflicting with any
# SDL3 the user already has installed via their distro's package manager.
#
# Instead we install the whole app into its own private prefix
# (/opt/3sx/{bin,lib,share}), which preserves the bin/../lib relationship
# the rpath expects, and only expose a symlink at /usr/bin/3sx so it's on
# PATH. Nothing outside /opt/3sx and that one symlink is touched.
#
# Usage:
#   ./tools/package-linux.sh <arch> <staging_dir> <short_sha> [output_dir]
#
# Example:
#   ./tools/package-linux.sh x86_64 dist a1b2c3d .
#
# Requires: fpm (https://fpm.readthedocs.io) installed and on PATH.

set -euo pipefail

if [[ $# -lt 3 ]]; then
    echo "Usage: $0 <arch> <staging_dir> <short_sha> [output_dir]" >&2
    exit 1
fi

ARCH="$1"
STAGING_DIR="$2"
SHORT_SHA="$3"
OUTPUT_DIR="${4:-.}"

PKG_NAME="3sx"
PKG_VERSION="0.2.0"
# FIX: This section should be updated when version tags are added
PKG_DESCRIPTION="3SX - Street Fighter III: 3rd Strike port"
PKG_URL="https://github.com/crowded-street/3sx"
PKG_LICENSE="AGPL-3.0"
INSTALL_PREFIX="/opt/3sx"

if [[ ! -d "${STAGING_DIR}" ]]; then
    echo "Staging directory '${STAGING_DIR}' not found. Did you run 'cmake --install'?" >&2
    exit 1
fi

mkdir -p "${OUTPUT_DIR}"

# --- Build a packaging root that mirrors the exact final filesystem layout ---
PKGROOT="$(mktemp -d)"
trap 'rm -rf "${PKGROOT}"' EXIT

mkdir -p "${PKGROOT}${INSTALL_PREFIX}"
cp -a "${STAGING_DIR}/." "${PKGROOT}${INSTALL_PREFIX}/"

# Expose the binary on PATH without touching /usr/lib.
# The rpath ($ORIGIN/../lib) still resolves correctly because the real
# file lives at /opt/3sx/bin/3sx -- $ORIGIN follows the symlink to its
# real location, not the symlink's own directory.
mkdir -p "${PKGROOT}/usr/bin"
ln -sf "${INSTALL_PREFIX}/bin/3sx" "${PKGROOT}/usr/bin/3sx"

common_fpm_args=(
    -s dir
    -n "${PKG_NAME}"
    -v "${PKG_VERSION}"
    --architecture "${ARCH}"
    --description "${PKG_DESCRIPTION}"
    --url "${PKG_URL}"
    --license "${PKG_LICENSE}"
    -C "${PKGROOT}"
    .
)

echo "==> Building .deb package"
fpm -t deb \
    --package "${OUTPUT_DIR}/3SX-${SHORT_SHA}-linux-${ARCH}.deb" \
    "${common_fpm_args[@]}"

echo "==> Building .rpm package"
fpm -t rpm \
    --package "${OUTPUT_DIR}/3SX-${SHORT_SHA}-linux-${ARCH}.rpm" \
    "${common_fpm_args[@]}"

echo "==> Building .pkg.tar.zst (Arch) package"
fpm -t pacman \
    --package "${OUTPUT_DIR}/3SX-${SHORT_SHA}-linux-${ARCH}.pkg.tar.zst" \
    "${common_fpm_args[@]}"

echo "==> Done. Packages written to ${OUTPUT_DIR}:"
ls -1 "${OUTPUT_DIR}"/3SX-"${SHORT_SHA}"-linux-"${ARCH}".{deb,rpm,pkg.tar.zst}
