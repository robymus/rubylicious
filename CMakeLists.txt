cmake_minimum_required(VERSION 3.15)

# We are building the rubylicious demo engine, yay!
project(Rubylicious)

# Meaningless message to avoid warning
if (CMAKE_TOOLCHAIN_FILE)
	message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")
endif()

# all dependencies
include(cmake/ruby.cmake)
include(cmake/sdl2.cmake)
include(cmake/opengl.cmake)

# finally the source
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(helloworld helloworld.c)

target_link_libraries(helloworld
	${RUBY_LIBS}
	${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES}
	${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
)
