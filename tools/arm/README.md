# ARM Tooling

Scripts for the ARM software-renderer ports live here.

- `build-deps.sh` cross-builds the third-party dependency tree into `third_party/armhf/`.
- `build-mister.sh` builds the MiSTer target in `build/mister/`.
- `build-rpi.sh` builds the Raspberry Pi DRM target in `build/rpi/`.

The wrapper scripts use Docker. That is the normal build path for these ARM
ports.

They create a reproducible Debian armhf toolchain and a bullseye-era sysroot
so the resulting binaries stay compatible with older glibc targets.

Release builds for these ARM ports leave the AFS checksum check off by
default. Hashing the full `SF33RD.AFS` file at startup is slow on lower-end
boards. If you want that check anyway, pass `-DCRS_ENABLE_AFS_CHECKSUM=ON`
when you configure the port with CMake.

Build outputs stay in the usual repo-local folders:

- `build/mister` and `build/mister-install`
- `build/rpi` and `build/rpi-install`
- `third_party/armhf`
