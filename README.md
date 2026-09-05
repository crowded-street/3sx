# 3SX

3SX is a native port of *Street Fighter III: 3rd Strike*. It's a continuation of [3s-decomp](https://github.com/crowded-street/3s-decomp) – a matching decompilation of 3rd Strike for PS2. 

> [!NOTE]
> 3SX is neither a recomp, nor emulation. It's native C code running directly on your hardware with no layers in between.

# Vision

For years fans have had to resort to less-than-ideal versions of 3S:
- Fightcade is great for playing online, but the fact that it's emulation-based makes it extremely hard (if not outright impossible) to add any meaningful features, aside from Lua training modes. It's also a pain to setup controllers for tournament play. And its second-class support for Linux and macOS makes it impossible to use in certain cases.
- 3S from the 30th Anniversary Collection is largely considered a subpar version, and its player base is very low what with the lack of crossplay.
- 3rd Strike Online Edition is beloved, yet flawed and abandoned. It's impossible to play on PC (outside of emulation) or modern consoles.

On the other hand, 3SX:
- brings first-class support for Windows, macOS and Linux (with possible ports to more platforms in the future)
- fixes arcade balance regressions introduced in PS2 version, providing a truly arcade-perfect experience (WIP)
- leverages GekkoNet to provide a fast, modern, cross-platform rollback netcode implementation

Our end goal with 3SX is to make **the ultimate version of 3rd Strike**: most responsive, most convenient, most easy-to-setup, most feature-rich, most played.

## Roadmap

Our goals for version 1.0:
- a stable PC experience
- easy access to online play
- auto-updates
- arcade ROM (legally obtained) as the only dependency

To track progress towards version 1.0 check our [1.0 milestone](https://github.com/crowded-street/3sx/milestone/1). To learn about other planned features check the [Backlog](https://github.com/crowded-street/3sx/milestone/2).

## How to play

> [!IMPORTANT]
> 3SX requires a legally obtained copy of *Street Fighter III: 3rd Strike* or *Street Fighter Anniversary Collection* for PlayStation 2 to run. This project does not condone piracy!

1. Download the latest release from the [Releases](https://github.com/mrksbrg/3sx/releases) page for this fork.
   CI build artifacts for the latest pull requests are also available from the [Actions](https://github.com/mrksbrg/3sx/actions) tab.

2. Follow startup wizard prompts to provide 3SX with your legally obtained copy of the PS2 version.

   _or_

   Manually copy <iso path>/THIRD/SF33RD.AFS to [`resources`](docs/resources.md) folder.

3. (Optional) Edit [config](docs/config.md) to adjust various settings to your liking.

## Documentation

All documentation (including the build guide) can be found in [docs](docs) folder.

## Local build helpers

For quick local branch testing, the repo includes convenience scripts for building and packaging the current checkout:

- `./build-local.sh` chooses the right local helper for the current OS.
- `./build-local-mac.sh` is the currently implemented macOS flow.

Only macOS support has been implemented so far. The pattern is simple to extend for Linux or Windows by following the same structure: bootstrap dependencies, configure the project with CMake, build the current branch, and then run the platform-specific packaging step.

These scripts are intended for local validation of the current branch and are not a replacement for the repo's release automation.

## CodeScene gate

The repository includes a local pre-commit guard that blocks commits when staged code degrades the CodeScene score or when newly added source code is not a perfect 10.0.

To enable it locally for this repo, run:

```bash
git config core.hooksPath .githooks
```

The hook calls `cs-mcp` from the CodeScene CLI. If the CLI is not installed, the hook exits with a clear message telling you to install it:

```bash
npm install -g @codescene/codehealth-mcp
cs-mcp --help
```

If you prefer not to use the global install, set `CS_MCP_BINARY_PATH` to the executable location before committing.

The hook also runs a short deterministic gameplay comparison when staged files
change `src/sf33rd/Source/Game`. It rebuilds `build-replay` and compares it with
a Debug baseline executable in `build-replay-main`. Create those build trees
from `main` and your working branch respectively:

```bash
CC=clang CXX=clang++ cmake -B build-replay-main -DCMAKE_BUILD_TYPE=Debug
cmake --build build-replay-main --parallel --config Debug

CC=clang CXX=clang++ cmake -B build-replay -DCMAKE_BUILD_TYPE=Debug
cmake --build build-replay --parallel --config Debug
```

The paths can be overridden with `THREESX_REPLAY_GUARD_BASELINE`,
`THREESX_REPLAY_GUARD_CANDIDATE`, and `THREESX_REPLAY_GUARD_BUILD_DIR`.
The hook recognizes macOS application bundles, Linux executables, and both
single- and multi-configuration Windows build layouts. On Windows it also
checks the standard MSYS2 MinGW64 location when `cmake` is not on `PATH`; set
`CMAKE` if the tool is installed elsewhere.
For broader validation, use `tools/compare_stress_replays.py` with multiple
seeds and longer frame counts as documented in `docs/statcheck.md`.

## Community

Join `Crowded Street` server on Discord to discuss the project, report bugs or share your ideas!

[![Join the Discord](https://dcbadge.limes.pink/api/server/https://discord.gg/wqs6BqYr8C)](https://discord.gg/wqs6BqYr8C)

## Acknowledgments

This project uses:
- [GekkoNet](https://github.com/HeatXD/GekkoNet) for P2P rollback netcode
- [SDL3](https://github.com/libsdl-org/SDL) for window management, input handling, sound output and rendering
- SDL_net for P2P connections
- [libcdio / libiso9660](https://github.com/libcdio/libcdio) for .iso file reading
- [zlib](https://zlib.net) for file decompression
- [argparse](https://github.com/cofyc/argparse) for parsing CLI arguments
- [minizip-ng](https://github.com/zlib-ng/minizip-ng) for unzipping
- [TF-PSA-Crypto](https://github.com/Mbed-TLS/TF-PSA-Crypto) for checksum calculation
- [stb](https://github.com/nothings/stb) for data structures
- [Dear ImGui](https://github.com/ocornut/imgui) for debug UI
