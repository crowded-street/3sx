# Build guide

## Setup

### Windows

1. Install [MSYS2](https://www.msys2.org/).
	* Steps after #4 on the official instructions can be skipped.
2. Launch the MinGW64 shell (there should be a start menu entry for it).
3. Install the required packages:

    ```bash
    pacman -S --needed $(cat tools/requirements-windows.txt)
    ```

### Linux

#### Ubuntu

```bash
sudo apt-get update
sudo apt-get install -y $(cat tools/requirements-ubuntu.txt)
```

### macOS

You should be able to build the project with just Xcode Command Line Tools.

1. Check if Command Line Tools are installed:

    ```bash
    xcode-select -p
    ```

2. Install if needed:

    ```bash
    xcode-select --install
    ```

## Building

1. Build dependencies

    ```bash
    sh build-deps.sh
    ```

2. Build the game

    ```bash
    CC=clang cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --parallel --config Release
    cmake --install build --prefix build/application
    ```

3. Copy from build/application to the desired location

## Software Renderer

The default desktop build uses the SDL GPU renderer. To build the software
renderer on desktop instead:

```bash
CC=clang CXX=clang++ cmake -B build-software -DCMAKE_BUILD_TYPE=Release -DCRS_SOFTWARE_RENDERER=ON
cmake --build build-software --parallel --config Release
cmake --install build-software --prefix build-software/application
```

## ARM Ports

The ARM ports use the software renderer and keep their outputs in the normal
repo-local build folders:

- `build/mister` for MiSTer
- `build/rpi` for Raspberry Pi DRM
- `build/mister-install` for the MiSTer install tree
- `build/rpi-install` for the Raspberry Pi install tree
- `third_party/armhf` for ARM cross-built dependencies

The provided ARM build scripts use Docker. You only need a working Docker
install on the host:

```bash
tools/arm/build-mister.sh
tools/arm/build-rpi.sh
```

Those scripts provision an `arm-linux-gnueabihf` cross toolchain and a
bullseye-era sysroot so the resulting binaries stay compatible with older
glibc targets. If you already have your own cross toolchain and sysroot you
can still drive CMake manually, but the normal path is to use the Docker
helpers above. Additional details are in [tools/arm/README.md](/mnt/e/development/3sx-software-renderer/tools/arm/README.md).

Release builds for MiSTer and Raspberry Pi leave the AFS checksum check off by
default. Reading and hashing the full `SF33RD.AFS` file at startup is slow on
lower-end ARM targets. If you want that check anyway, pass
`-DCRS_ENABLE_AFS_CHECKSUM=ON` when configuring.

### MiSTer runtime layout

After running `tools/arm/build-mister.sh`, copy the installed files from
`build/mister-install` to a directory on the MiSTer, for example
`/media/fat/games/3sx/`:

```text
bin/3sx
lib/
resources/SF33RD.AFS
share/3sx/licenses/    # optional
```

The binary uses `../lib` as its runtime library path, so keep `bin/3sx` and
`lib/` together.

### Raspberry Pi runtime layout

After running `tools/arm/build-rpi.sh`, copy the installed files from
`build/rpi-install` to a directory on the Pi, for example `~/3sx/`:

```text
bin/3sx
lib/
resources/SF33RD.AFS
share/3sx/licenses/    # optional
```

The binary uses `../lib` as its runtime library path, so keep `bin/3sx` and
`lib/` together.

If you switch the Pi back to its normal HDMI mode list so the app can pick a
`1920x1080` output mode, HDMI audio routing can change as well. If audio stops
coming out of HDMI after that change, check the Pi audio output setting before
debugging the port itself.

For both MiSTer and Raspberry Pi, if `THREESX_HOME` is set to that app
directory, the ARM ports will load the AFS directly from:

```text
<THREESX_HOME>/resources/SF33RD.AFS
```

Without `THREESX_HOME`, the fallback remains:

```text
~/.local/share/CrowdedStreet/3SX/resources/SF33RD.AFS
```

Config still uses the normal pref path:

```text
~/.local/share/CrowdedStreet/3SX/config
```
