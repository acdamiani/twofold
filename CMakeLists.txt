cmake_minimum_required(VERSION 3.12)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    message(
        FATAL_ERROR
        "Twofold: You cannot build in a source directory (or any directory with "
        "CMakeLists.txt file). Please make a build subdirectory. Feel free to "
        "remove CMakeCache.txt and CMakeFiles."
    )
endif()

project(Twofold VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_subdirectory(src)
add_subdirectory(third_party)
