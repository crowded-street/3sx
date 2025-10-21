# GekkoNet
### C/C++ Peer To Peer Game Networking SDK
Traditional online networking techniques account for transmission delays by adding input lag, often resulting in a slow feeling, unresponsive feel. GekkoNet leverages rollback networking with input prediction and speculative execution, allowing player inputs to be processed immediately, creating a seamless, low-latency experience. This means that players enjoy consistent timing, reaction speeds, and muscle memory both online and offline, without the impact of added network lag. Inspired by [GGPO](https://github.com/pond3r/ggpo) and [GGRS](https://github.com/gschup/ggrs)

## Why?
I built this because I wanted a SDK to plug into my C++ projects, in the past I have created a wrapper around GGRS for C++ but after having to deal with Rust FFI I decided to build a native alternative instead to more easily fit my projects. 
GekkoNet is heavily inspired by the [GGPO](https://github.com/pond3r/ggpo) Rust reimplementation [GGRS](https://github.com/gschup/ggrs).

#### Why not use GGPO?
I am personally not a big fan of the callback based approach of GGPO hence why I am more of fond of how GGRS handles its control flow. And I might be addicted to reinventing the wheel, this has mostly been a learning experience of mine to learn more about async systems and networking in general :)

## Project Goals
### Done
- Local/Couch Sessions
	- Per Player Input Delay Settings
- Online Sessions
	- Local Player Input Delay Settings
	- Remote Player Input Prediction Settings
- Spectator Sessions
	- Spectator Delay Settings.
	- The added ability to spectate spectators. This might be handy if you have a seperate spectating service which propegates the inputs to more spectators.
- Limited Saving 
	- Save the gamestate less often which might help games where saving the game is expensive. This is at the cost of more iterations advancing the gamestate during rollback.
- Abstracted socket manager.
- Event System for notifications for eg. specific players being done with syncing.
- Desync Detection (Only when limited saving is disabled for now)
- Automated builds
- Network Statistics

### Work in progress
- Joining a session that's already in progress as a spectator (and maybe as a player later)

### Maybe Later
- Replays
- Game engine plugins
- Commission an artist to create a logo for GekkoNet

## Getting Started
### Docs
- Automatically generated Docs: https://heatxd.github.io/GekkoNet/
- Also look at the examples to see how GekkoNet functions!

## Building Examples
- The examples are built and ran using Visual Studio 2022
- You will need installed SDL2 as prerequisite
  
## Building GekkoLib
### Prerequisites
To build GekkoNet, make sure you have the following installed:

1. **CMake** (version 3.15 or higher)
2. **C++ Compiler**:
   - **GCC** or **Clang** (Linux/macOS)
   - **MSVC** (Visual Studio) for Windows
3. **Doxygen** (optional, for documentation generation if `BUILD_DOCS` is enabled)

### Step-by-Step Instructions

### 1. Clone the Repository
First, clone the GekkoNet repository:

```sh
git clone https://github.com/HeatXD/GekkoNet.git
cd GekkoNet/GekkoLib
```

#### 2. Configure Build Options
GekkoNet includes several options to customize the build:

- `BUILD_SHARED_LIBS`: Set to `ON` to build shared libraries, or `OFF` for static libraries (default).
- `NO_ASIO_BUILD`: Set to `ON` if you do not need ASIO.
- `BUILD_DOCS`: Set to `ON` if you want to generate documentation using Doxygen (requires Doxygen installed).

To configure these options, use `cmake` with `-D` flags. For example:

```sh
cmake -S . -B build -DBUILD_SHARED_LIBS=ON -DNO_ASIO_BUILD=OFF -DBUILD_DOCS=OFF
```

### 3. Generate Build Files
Run CMake to configure the build and generate files:

```sh
cmake -S . -B build
```

#### 4. Build the Project
Once configured, build the project using the following command:

```sh
cmake --build build
```

On successful completion, binaries and libraries will be located in the `out` directory within the project.

#### 5. (Optional) Build Documentation
If you set `BUILD_DOCS=ON`, generate the documentation as follows:

```sh
cmake --build build --target docs
```

Documentation will be available in the `build/docs` directory as HTML files.

### Build Output
- **Library**: Located in `out/`, with shared/static suffixes depending on build options.
- **Documentation** (if built): Available in `build/docs/`.

---

## Projects using GekkoNet
- GekkoNet SteamPort by @lolriley
	- https://github.com/lolriley/GekkoNet/tree/steamport


If you have a project using GekkoNet please let me know!

## License
GekkoNet is licensed under the BSD-2-Clause license
[Read about it here](https://opensource.org/license/bsd-2-clause).
