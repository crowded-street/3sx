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
    CC=clang CXX=clang++ cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --parallel --config Release
    cmake --install build --prefix build/application
    ```

    GekkoNet netplay is enabled by default for desktop builds, including Release.
    To disable it, configure with `-DTHREESX_NETPLAY=OFF`.

    This custom build defaults to Fistbump at `3.16.58.140` (TCP 9000,
    UDP 9001). Selecting Arcade connects in the background, displays browser
    activation details when login is needed, queues after login, and automatically
    accepts matches. A match ends the CPU run through the reset path before
    entering network character select. Returning to mode select cancels the
    search; connection errors leave Arcade running offline. This is the initial
    auto-accept testing flow, without an accept/decline dialog in Arcade.

    Override the compiled endpoint with `-DTHREESX_FISTBUMP_HOST=host`,
    `-DTHREESX_FISTBUMP_TCP_PORT=9000`, and `-DTHREESX_FISTBUMP_UDP_PORT=9001`.
    At launch, `--matchmaking-ip` and `--matchmaking-port` independently override
    the default host and TCP port. Explicit `--p2p-*` arguments retain direct
    connection mode and disable Arcade matchmaking.

    On macOS, after a Makefiles Release build, run
    `python3 tools/tests/run_arcade_matchmaking_smoke.py` to test the production
    matchmaking code with local mock servers and temporary login profiles.
    This covers the handoff to the reset state, not a rendered CPU-to-network
    transition or a live two-player match.

    Enable replay statcheck independently of the build configuration. For example,
    use `RelWithDebInfo` for routine runs:

    ```bash
    CC=clang CXX=clang++ cmake -B build-statcheck -DCMAKE_BUILD_TYPE=RelWithDebInfo -DTHREESX_STATCHECK=ON
    cmake --build build-statcheck --parallel --config RelWithDebInfo
    ```

    For debugger-friendly statcheck runs, configure a Debug build instead:

    ```bash
    CC=clang CXX=clang++ cmake -B build-statcheck-debug -DCMAKE_BUILD_TYPE=Debug -DTHREESX_STATCHECK=ON
    cmake --build build-statcheck-debug --parallel --config Debug
    ```

3. Copy from build/application to the desired location
