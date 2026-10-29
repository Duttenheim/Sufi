# Sufi
# Main CMakeLists

cmake_minimum_required(VERSION 4.0)

include(${CMAKE_CURRENT_LIST_DIR}/macros.cmake)

if(NOT SUFI_IMPORT)
    project(Sufi)
endif()

# One platform definition for the system being built. iOS is Darwin too,
# so it is matched before macOS.
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(WINDOWS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    add_compile_definitions(IOS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_compile_definitions(MACOS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(LINUX)
endif()

add_subdirectory(code)
add_subdirectory(samples)