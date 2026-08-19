#!/bin/bash
# Ensure MSYS2 environment is loaded
export PATH=/usr/bin:/mingw64/bin:$PATH

# Clean build directory (optional - removes old cache)
# rm -rf build

# Configure with policy set
CC=clang CXX=clang++ cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_POLICY_DEFAULT_CMP0207=NEW -Wno-dev

# Build
cmake --build build --parallel --config Debug

# Install
cmake --install build --prefix build/application