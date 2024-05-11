cmake_minimum_required(VERSION 3.16)

project(TuringSurakarta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
elseif (NOT CMAKE_BUILD_TYPE MATCHES "Debug|Release")
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/surakarta_ta)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-O0 -g -fsanitize=address -Wall -Wextra")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra")
endif()

set(ENV "TEST" CACHE STRING "DEV or TEST, for students, please use TEST")

add_subdirectory(src)

