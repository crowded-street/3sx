# Build guide

## Setup

### Windows

1. Install [MSYS2](https://https://www.msys2.org/).
	* Steps after #4 on the official instructions can be skipped.
2. Launch the MinGW64 shell (there should be a start menu entry for it).
3. Install the required packages:

    ```bash
    pacman -S make mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-nasm mingw-w64-x86_64-clang mingw-w64-x86_64-headers-git
    ```

### Linux

#### Ubuntu

```bash
sudo apt-get update
sudo apt-get install -y $(cat tools/requirements-ubuntu-sdl3.txt)
sudo apt-get install -y clang curl nasm
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
