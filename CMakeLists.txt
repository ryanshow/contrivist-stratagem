cmake_minimum_required (VERSION 2.8)
project (ContrivistStratagem)

set (CS_VERSION_MAJOR 0)
set (CS_VERSION_MINOR 1)

# Get the git revision hash upon build time
find_package (Git)
file (WRITE ${CMAKE_BINARY_DIR}/main.c
    "\#include \"version.h\"\nint main(void){return 0;}\n"
)
file (WRITE ${CMAKE_BINARY_DIR}/git_sha1.h.in
    "\#define GIT_SHA1 \"@GIT_SHA1@\"\n"
)
file (WRITE ${CMAKE_BINARY_DIR}/git_sha1.cmake
    "EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_SHA1
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    CONFIGURE_FILE(\${SRC} \${DST} @ONLY)
")

add_custom_target (
    version
    ${CMAKE_COMMAND} -D SRC=${CMAKE_BINARY_DIR}/git_sha1.h.in
                     -D DST=${CMAKE_BINARY_DIR}/git_sha1.h
                     -P ${CMAKE_BINARY_DIR}/git_sha1.cmake
)

configure_file (
    "${PROJECT_SOURCE_DIR}/src/version.h.in"
    "${PROJECT_BINARY_DIR}/version.h"
)

include (CheckCXXCompilerFlag)
check_cxx_compiler_flag (-std=c++11 HAVE_STD_CPP11_FLAG)

# Compile GLFW
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
SET(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
add_subdirectory (extern/glfw3)

# Compile Format
add_subdirectory (extern/format)

# GLEW is pretty much impossible to compile directly with CMake.
find_package(GLEW REQUIRED)

include_directories (
    extern/format
    extern/glfw3/include
    extern/glm
    ${GLEW_INCLUDE_PATH}
    ${PROJECT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
)

set (CS_SOURCE src/main.cpp
)

add_executable (cs ${CS_SOURCE})

if (HAVE_STD_CPP11_FLAG)
    set_target_properties (cs PROPERTIES COMPILE_FLAGS "-std=c++11")
endif ()

message (STATUS "${GLFW_LIBRARIES}")

target_link_libraries (cs format glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARY})

add_dependencies (cs version)
