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

include_directories (
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/extern/format
    ${PROJECT_SOURCE_DIR}/extern/glfw3/include
    ${PROJECT_SOURCE_DIR}/extern/glm
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
add_definitions(-DGLM_SWIZZLE)
#add_definitions(-DGL_GLEXT_PROTOTYPES)
add_definitions(-DGLFW_INCLUDE_GLCOREARB)

target_link_libraries (cs format glfw ${GLFW_LIBRARIES})

add_dependencies (cs version)

if (UNIX OR APPLE)
    execute_process (
        COMMAND ln -sf ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}
    )
elseif (WIN32)
    execute_process (
        COMMAND mklink /D ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/assets
    )
endif ()
