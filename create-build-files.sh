#!/bin/sh
set -f
cmake -B build/linux -G Ninja
cmake -B build/win32 -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/i686-w64-mingw32.cmake
