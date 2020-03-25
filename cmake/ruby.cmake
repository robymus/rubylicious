#
# Configuration for ruby
#

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
