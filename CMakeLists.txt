cmake_minimum_required(VERSION 3.1...3.16)

project(rubylicious
	VERSION 1.0
	DESCRIPTION "The rubylicious demo engine"
	LANGUAGES C CXX)

# Meaningless message to avoid warning
if (CMAKE_TOOLCHAIN_FILE)
	message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")
endif()

# all dependencies
include(cmake/ruby.cmake)
include(cmake/sdl2.cmake)
include(cmake/opengl.cmake)

# header only libraries
include_directories(lib/glm)

# finally the code
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(rubylicious
	src/helloworld.c
)

target_link_libraries(rubylicious
	${RUBY_LIBS}
	${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES}
	${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
)
