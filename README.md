# 3SX

3SX is a native port of *Street Fighter III: 3rd Strike* that focuses on modern platforms.

## Features

| Feature | Status |
| ---- | ------ |
| Provide a convenient and accessible way to run 3S natively on modern platforms | Runs on Windows, macOS, Linux |
| Add quality-of-life improvements | ✅ |
| Enable cross-platform rollback netplay with matchmaking and custom rooms | WIP |
| Enable porting the game to niche platforms | WIP |

## How to play

3SX requires an official copy of *Street Fighter III: 3rd Strike* or *Street Fighter Anniversary Collection* for PlayStation 2 to run.

1. Download the latest release from the [Releases](https://github.com/crowded-street/3sx/releases) page.
2. Follow startup wizard prompts to provide 3SX with your legally owned copy of the PS2 version.
3. (Optional) Edit [config](docs/config.md) to adjust various settings to your liking.

## Building

See the [build guide](docs/building.md).

## Community

Join `Crowded Street` server on Discord to discuss the project, report bugs or share your ideas!

[![Join the Discord](https://dcbadge.limes.pink/api/server/https://discord.gg/wqs6BqYr8C)](https://discord.gg/wqs6BqYr8C)

## Acknowledgments

This project uses:
- [GekkoNet](https://github.com/HeatXD/GekkoNet) for P2P rollback netcode
- [FFmpeg](https://ffmpeg.org) for ADX playback
- [SDL3](https://github.com/libsdl-org/SDL) for window management, input handling, sound output and rendering
- SDL_net for P2P connections
- [libcdio / libiso9660](https://github.com/libcdio/libcdio) for .iso file reading
- [zlib](https://zlib.net) for file decompression
- [argparse](https://github.com/cofyc/argparse) for parsing CLI arguments
- [minizip-ng](https://github.com/zlib-ng/minizip-ng) for unzipping
- [TF-PSA-Crypto](https://github.com/Mbed-TLS/TF-PSA-Crypto) for checksum calculation
- [stb](https://github.com/nothings/stb) for data structures
- [Dear ImGui](https://github.com/ocornut/imgui) for debug UI
