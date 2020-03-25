# Compile dependencies

These are the packages needed on Fedora.

## Linux only

Libraries: SDL2, SDL2_image

`sudo dnf install SDL2-devel SDL2_image-devel`

Build tools

cmake, ninja

`sudo dnf install cmake ninja-build`

gcc/g++, but you have those anyway :)

## Mingw32 cross compiling

Cross compiler toolchain - see [RubyCompile.md](CompileRuby.md)


Libraries: SDL2, SDL2_image

` sudo dnf install mingw32-SDL2 mingw32-SDL2_image`
