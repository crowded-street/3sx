#!/usr/bin/env bash
set -euo pipefail

TARGET="${1:-}"
shift $(( $# > 0 ? 1 : 0 ))

if [ -z "${TARGET}" ]; then
    echo "usage: $0 <mister|rpi> [--clean] [--jobs N] [--container NAME]" >&2
    exit 2
fi

case "${TARGET}" in
    mister)
        BUILD_DIR="build/mister"
        INSTALL_DIR="build/mister-install"
        CMAKE_TARGET_FLAG="-DCRS_MISTER=ON"
        ARM_CPU_FLAGS="-mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard"
        ;;
    rpi)
        BUILD_DIR="build/rpi"
        INSTALL_DIR="build/rpi-install"
        CMAKE_TARGET_FLAG="-DCRS_RPI_DRM=ON"
        ARM_CPU_FLAGS="-march=armv7-a -mfpu=neon -mfloat-abi=hard"
        ;;
    *)
        echo "unknown target: ${TARGET}" >&2
        exit 2
        ;;
esac

CLEAN=0
CONTAINER_NAME="${ARM_BUILD_CONTAINER:-3sx-arm-build}"
PLATFORM="${ARM_DOCKER_PLATFORM:-linux/amd64}"
BUILD_IMAGE="${ARM_BUILD_IMAGE:-debian:trixie}"
JOBS="${JOBS:-$(nproc 2>/dev/null || echo 4)}"

while [ "$#" -gt 0 ]; do
    case "$1" in
        --clean) CLEAN=1; shift ;;
        --jobs) JOBS="$2"; shift 2 ;;
        --container) CONTAINER_NAME="$2"; shift 2 ;;
        *) echo "unknown option: $1" >&2; exit 2 ;;
    esac
done

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*)
        ROOT_DIR_HOST="$(cygpath -w "${ROOT_DIR}")"
        ;;
    *)
        ROOT_DIR_HOST="${ROOT_DIR}"
        ;;
esac

command -v docker >/dev/null 2>&1 || { echo "docker is required" >&2; exit 2; }

if docker ps -a --format '{{.Names}}' | grep -qx "${CONTAINER_NAME}"; then
    docker start "${CONTAINER_NAME}" >/dev/null
else
    docker run -d --name "${CONTAINER_NAME}" --platform "${PLATFORM}" \
        -v "${ROOT_DIR_HOST}":/src -w /src \
        "${BUILD_IMAGE}" sleep infinity >/dev/null
fi

docker exec "${CONTAINER_NAME}" bash -lc '
set -euo pipefail
if [ ! -f /opt/.3sx-arm-provisioned ]; then
    export DEBIAN_FRONTEND=noninteractive
    apt-get update
    apt-get install -y --no-install-recommends \
        build-essential ca-certificates ccache cmake curl git make nasm pkg-config \
        perl python3 python3-pip rsync xz-utils libssl-dev
    dpkg --add-architecture armhf
    apt-get update
    apt-get install -y --no-install-recommends \
        gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf binutils-arm-linux-gnueabihf \
        libc6-dev-armhf-cross libasound2-dev:armhf \
        libdrm-dev:armhf
    if ! cmake --version | awk '"'"'NR==1 { split($3, v, "."); exit !((v[1] > 3) || (v[1] == 3 && v[2] >= 24)) }'"'"'; then
        python3 -m pip install --no-cache-dir "cmake>=3.24,<4"
    fi
    touch /opt/.3sx-arm-provisioned
fi

if [ ! -f /opt/.3sx-arm-sysroot-ready ]; then
    mkdir -p /opt/sysroot-bullseye-armhf /tmp/bullseye-debs
    cd /tmp/bullseye-debs
    cat >/tmp/bullseye-armhf.list <<EOF
deb [arch=armhf] http://archive.debian.org/debian bullseye main
deb [arch=armhf] http://archive.debian.org/debian bullseye-updates main
EOF
    apt-get -o Dir::Etc::sourcelist=/tmp/bullseye-armhf.list \
        -o Dir::Etc::sourceparts=/dev/null \
        -o Acquire::Check-Valid-Until=false \
        update
    apt-get -o Dir::Etc::sourcelist=/tmp/bullseye-armhf.list \
        -o Dir::Etc::sourceparts=/dev/null \
        -o Acquire::Check-Valid-Until=false \
        download \
        libc6:armhf=2.31-13+deb11u11 \
        libc6-dev:armhf=2.31-13+deb11u11 \
        linux-libc-dev:armhf=5.10.223-1 \
        libasound2:armhf=1.2.4-1.1 \
        libasound2-dev:armhf=1.2.4-1.1 \
        libdrm2:armhf=2.4.104-1 \
        libdrm-dev:armhf=2.4.104-1
    for f in ./*.deb; do
        dpkg-deb -x "$f" /opt/sysroot-bullseye-armhf
    done
    touch /opt/.3sx-arm-sysroot-ready
fi

if [ ! -f /opt/.3sx-arm-libdrm-ready ]; then
    rm -f /opt/sysroot-bullseye-armhf/usr/lib/arm-linux-gnueabihf/libdrm.so*
    rm -f /opt/sysroot-bullseye-armhf/usr/lib/arm-linux-gnueabihf/pkgconfig/libdrm.pc
    rm -f /opt/sysroot-bullseye-armhf/usr/include/xf86drm.h /opt/sysroot-bullseye-armhf/usr/include/xf86drmMode.h
    rm -rf /opt/sysroot-bullseye-armhf/usr/include/libdrm

    mkdir -p /tmp/bullseye-debs
    cd /tmp/bullseye-debs
    rm -f libdrm2_*_armhf.deb libdrm-dev_*_armhf.deb
    apt-get -o Dir::Etc::sourcelist=/tmp/bullseye-armhf.list \
        -o Dir::Etc::sourceparts=/dev/null \
        -o Acquire::Check-Valid-Until=false \
        download \
        libdrm2:armhf=2.4.104-1 \
        libdrm-dev:armhf=2.4.104-1
    for f in libdrm2_*_armhf.deb libdrm-dev_*_armhf.deb; do
        dpkg-deb -x "$f" /opt/sysroot-bullseye-armhf
    done
    touch /opt/.3sx-arm-libdrm-ready
fi

mkdir -p /usr/arm-linux-gnueabihf
if [ ! -L /usr/arm-linux-gnueabihf/include ]; then
    rm -rf /usr/arm-linux-gnueabihf/include.bak
    if [ -e /usr/arm-linux-gnueabihf/include ]; then
        mv /usr/arm-linux-gnueabihf/include /usr/arm-linux-gnueabihf/include.bak
    fi
    ln -s /opt/sysroot-bullseye-armhf/usr/include /usr/arm-linux-gnueabihf/include
fi

if [ ! -f /opt/.3sx-arm-cross-lib-ready ]; then
    ORIG_CROSS_LIB_DIR=/usr/arm-linux-gnueabihf/lib.bak
    if [ -d /usr/arm-linux-gnueabihf/lib.trixie ]; then
        ORIG_CROSS_LIB_DIR=/usr/arm-linux-gnueabihf/lib.trixie
    fi

    rm -rf /usr/arm-linux-gnueabihf/lib.bak
    if [ -d /usr/arm-linux-gnueabihf/lib ] && [ ! -L /usr/arm-linux-gnueabihf/lib ]; then
        mv /usr/arm-linux-gnueabihf/lib /usr/arm-linux-gnueabihf/lib.bak
        ORIG_CROSS_LIB_DIR=/usr/arm-linux-gnueabihf/lib.bak
    else
        rm -f /usr/arm-linux-gnueabihf/lib
    fi

    mkdir -p /usr/arm-linux-gnueabihf/lib

    for src in /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/* \
               /opt/sysroot-bullseye-armhf/usr/lib/arm-linux-gnueabihf/*; do
        ln -sf "$src" "/usr/arm-linux-gnueabihf/lib/$(basename "$src")"
    done

    for src in "$ORIG_CROSS_LIB_DIR"/libgcc_s.so.1 \
               "$ORIG_CROSS_LIB_DIR"/libstdc++.so.6 \
               "$ORIG_CROSS_LIB_DIR"/libstdc++.so.* \
               "$ORIG_CROSS_LIB_DIR"/libatomic.so.* \
               "$ORIG_CROSS_LIB_DIR"/libgomp.so.* \
               "$ORIG_CROSS_LIB_DIR"/libubsan.so.* \
               "$ORIG_CROSS_LIB_DIR"/libasan.so.*; do
        [ -e "$src" ] || continue
        ln -sf "$src" "/usr/arm-linux-gnueabihf/lib/$(basename "$src")"
    done

    # Debian bullseye glibc ships several linker names in usr/lib as symlinks
    # to /lib/arm-linux-gnueabihf/*. In the container those absolute symlinks
    # would resolve against the host root, not the sysroot, so point them
    # straight at the matching sysroot DSOs.
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libpthread.so.0 \
        /usr/arm-linux-gnueabihf/lib/libpthread.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libdl.so.2 \
        /usr/arm-linux-gnueabihf/lib/libdl.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libm.so.6 \
        /usr/arm-linux-gnueabihf/lib/libm.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/librt.so.1 \
        /usr/arm-linux-gnueabihf/lib/librt.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libutil.so.1 \
        /usr/arm-linux-gnueabihf/lib/libutil.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libanl.so.1 \
        /usr/arm-linux-gnueabihf/lib/libanl.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libBrokenLocale.so.1 \
        /usr/arm-linux-gnueabihf/lib/libBrokenLocale.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libresolv.so.2 \
        /usr/arm-linux-gnueabihf/lib/libresolv.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libnss_compat.so.2 \
        /usr/arm-linux-gnueabihf/lib/libnss_compat.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libnss_dns.so.2 \
        /usr/arm-linux-gnueabihf/lib/libnss_dns.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libnss_files.so.2 \
        /usr/arm-linux-gnueabihf/lib/libnss_files.so
    ln -snf /opt/sysroot-bullseye-armhf/lib/arm-linux-gnueabihf/libnss_hesiod.so.2 \
        /usr/arm-linux-gnueabihf/lib/libnss_hesiod.so

    touch /opt/.3sx-arm-cross-lib-ready
fi
'

if [ "${CLEAN}" = "1" ]; then
    docker exec "${CONTAINER_NAME}" rm -rf "/src/${BUILD_DIR}" "/src/${INSTALL_DIR}"
fi

docker exec -e JOBS="${JOBS}" \
    -e CRS_ARM_CPU_FLAGS="${ARM_CPU_FLAGS}" \
    -e CRS_ARM_SYSROOT="/opt/sysroot-bullseye-armhf" \
    "${CONTAINER_NAME}" \
    bash /src/tools/arm/build-deps.sh

docker exec "${CONTAINER_NAME}" bash -lc "
set -euo pipefail
export PKG_CONFIG_LIBDIR=/opt/sysroot-bullseye-armhf/usr/lib/arm-linux-gnueabihf/pkgconfig:/opt/sysroot-bullseye-armhf/usr/share/pkgconfig:/usr/lib/pkgconfig:/usr/share/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=/opt/sysroot-bullseye-armhf
cmake -S /src -B /src/${BUILD_DIR} \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/src/cmake/toolchain-armhf.cmake \
    -DCRS_ARM_CPU_FLAGS='${ARM_CPU_FLAGS}' \
    -DCRS_ARM_SYSROOT=/opt/sysroot-bullseye-armhf \
    ${CMAKE_TARGET_FLAG}
cmake --build /src/${BUILD_DIR} --parallel ${JOBS}
cmake --install /src/${BUILD_DIR} --prefix /src/${INSTALL_DIR}
"
