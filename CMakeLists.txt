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

# Compile freetype-gl
SET(freetype-gl_BUILD_DEMOS OFF CACHE BOOK "Build the freetype-gl example programs")
add_subdirectory (extern/freetype-gl)

# Compile Chaiscript
SET(BUILD_MODULES OFF CACHE BOOL "Build Extra Modules")
SET(BUILD_SAMPLES OFF CACHE BOOL "Build Samples Folder")
SET(BUILD_TESTING OFF CACHE BOOL "Build Unit Tests")
add_subdirectory (extern/chaiscript)

# Compile Format
add_subdirectory (extern/format)

include_directories (
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/extern/format
    ${PROJECT_SOURCE_DIR}/extern/glfw3/include
    ${PROJECT_SOURCE_DIR}/extern/glm
    ${PROJECT_SOURCE_DIR}/extern/gli
    ${PROJECT_SOURCE_DIR}/extern/chaiscript/include
    ${PROJECT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
)

file (GLOB COMMON_SOURCE "src/*.cpp")

file (GLOB COMMON_HEADERS "src/*.h")

add_executable (cs ${COMMON_SOURCE} ${COMMON_HEADERS})

if (HAVE_STD_CPP11_FLAG)
    set_target_properties (cs PROPERTIES COMPILE_FLAGS "-std=c++11")
endif ()

add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-DGLFW_INCLUDE_GLCOREARB)

target_link_libraries (cs format glfw ${GLFW_LIBRARIES})

add_dependencies (cs version)

file (
    COPY assets
    DESTINATION ./
)
