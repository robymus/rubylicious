# Rubylicious
## The Rubylicious Demo Engine
### soon :)

hopefully.

## What is it? How does it work?

The basic idea is that a demo will be scripted from Ruby, with the low level functions implemented in a C++ engine.

This project comes with a sample demo that exercises all the exposed API functions.

The main functions are exposed through a Demo class, actually its class methods (eg. singleton methods) to Ruby.

Check out the [API documentation](apidoc/README.md).

## Compiling

It's designed to be developed on Linux, with crosscompiling to win32. It might be possible to compile on native Windows, but the dependencies might not work as they are now. 

`./create-build-files.sh` to create the ninja build files in build/

`./b-linux.sh` or `./b-win.sh` to build the binaries (stored in `build/(linux|win32)`)

See [package dependencies](doc/Dependencies.md) and info on [compiling ruby](doc/CompileRuby.md) (optional, you can use the precompiled binaries).

## Distribution

On windows, the additional dll files from `arch/i386-mingw32/dll` are required. 

Currently only the .png external file format is added to SDL2_image - if you need additional image formats, add the required DLL files as described at https://www.libsdl.org/projects/SDL_image/ (also modification in the code is necessary - in call to IMG_INIT).

Similarly, only MP3 format is added for SDL2_mixer.

TODO: automatic creation of distribution package, linux/windows as well (collect dll/so)
