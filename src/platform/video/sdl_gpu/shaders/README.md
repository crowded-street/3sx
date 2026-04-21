# Shaders

This folder contains shaders for use with the SDL GPU renderer.

## Updating shaders

HLSL shaders are not transpiled into platform-specific formats automatically. Shader translation requires building SDL_shadercross and its dependencies. If they were a part of the normal build pipeline, both local and CI build times would increase dramatically.

To update shaders/add new ones:
1. Update/create an HLSL shader
2. Build SDL_shadercross using `tools/build-shadercross.sh`
3. Run `tools/transpile-shaders.sh`

> Transpiling to MSL on Linux/Windows is very clunky, and I haven't tested the workflow myself. If you don't have access to a macOS machine, ping me (apstygo), and I'll help you with MSL
