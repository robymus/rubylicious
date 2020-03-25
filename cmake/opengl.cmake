#
# Configuration for opengl + glew
#

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# use static glew on mingw
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(GLEW_LIBRARIES GLEW::glew_s)
else()
    set(GLEW_LIBRARIES GLEW::glew)
endif()
