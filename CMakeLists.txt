cmake_minimum_required(VERSION 3.16)

# We are building the rubylicious demo engine, yay!
project(Rubylicious)

# Let's set up our ruby version to use
set(RUBY_VERSION 2.7.0)

# Add ruby library + extra dependencies to linker
# Also set required RUBY_ARCH
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(RUBY_LIBS msvcrt-ruby270-static shell32 ws2_32 iphlpapi imagehlp shlwapi)	
	set(RUBY_ARCH i386-mingw32)
else()
	set(RUBY_LIBS ruby-static z pthread rt gmp dl crypt m)	
	set(RUBY_ARCH x86_64-linux)
endif()

# Set ruby include / library paths
include_directories("arch/${RUBY_ARCH}/ruby-${RUBY_VERSION}/include" "arch/${RUBY_ARCH}/ruby-${RUBY_VERSION}/include/${RUBY_ARCH}")
link_directories("arch/${RUBY_ARCH}/ruby-${RUBY_VERSION}/lib")

# Find SDL2
# using SDL2 modules from https://github.com/aminosbh/sdl2-cmake-modules
list(APPEND CMAKE_MODULE_PATH cmake/sdl2)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(helloworld helloworld.c)
target_link_libraries(helloworld ${RUBY_LIBS} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
