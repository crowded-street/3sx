# 3SX

A port of the greatest fighting game of all time for modern platforms.

Requires an official copy of *Street Fighter III: 3rd Strike* or *Street Fighter Anniversary Collection* for PlayStation 2 to play.

Based on a [decompilation](https://github.com/crowded-street/sfiii-decomp) of the PlayStation 2 port.

## Resources

Find instructions on [how to build](docs/building.md) the project and other useful resources in the [docs](docs) folder.

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
