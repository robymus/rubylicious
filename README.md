# rubylicious
## the rubylicious demo engine
### soon :)

hopefully.

## Compiling

`./create-build-files.sh` to create the ninja build files in build/
`./b-linux.sh` or `./b-win.sh` to build the binaries (stored in `build/(linux|win32)`)

See [package dependencies](doc/Dependencies.md) and info on [compiling ruby](doc/CompileRuby.md) (optional, you can use the precompiled binaries).

## Distribution

On windows, the additional dll files from `arch/i386-mingw32/dll` are required. Currently only the .png external file format is added to sdl_image - if you need additional image formats, add the required DLL files as described at https://www.libsdl.org/projects/SDL_image/ (also modification in the code is necessary - in call to IMG_INIT).

TODO: automatic creation of distribution package, linux/windows as well
