cmake_minimum_required (VERSION 2.8.12.2)
project (ContrivistStratagem)

set (CS_VERSION_MAJOR 0)
set (CS_VERSION_MINOR 1)

configure_file (
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
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
)

set (CS_SOURCE src/main.cpp
)

add_executable (cs ${CS_SOURCE})

if (HAVE_STD_CPP11_FLAG)
    set_target_properties (cs PROPERTIES COMPILE_FLAGS "-std=c++11")
endif ()

message (STATUS "${GLFW_LIBRARIES}")

target_link_libraries (cs format glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARY})
