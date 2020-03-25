set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR i686)
set(TOOLCHAIN_PREFIX i686-w64-mingw32)

set(CMAKE_C_COMPILER     "${TOOLCHAIN_PREFIX}-gcc")
set(CMAKE_CXX_COMPILER   "${TOOLCHAIN_PREFIX}-g++")

set(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32/sys-root/mingw)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_CROSS_COMPILING TRUE)
