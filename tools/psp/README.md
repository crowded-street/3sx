# PSP Tooling

PSP build scripts meant to run inside a Docker container.

Build SDL3 and minizip-ng with the PSP toolchain before building the game:

```sh
./tools/psp/build-sdl.sh
./tools/psp/build-minizip.sh
./tools/psp/build-port.sh
```

The dependency scripts install PSP libraries into `third_party`, using the same
paths as desktop builds. Use a separate checkout for PSP builds to keep the
platform libraries separate.
