cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    project(HospitalTriage VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_TESTS "Build unit tests" ON)
option(USE_SQLITE "Use SQLite for persistence" OFF)

# Connecting external dependencies
include(cmake/Dependencies.cmake)

# Library
add_subdirectory(src/backend)

# Executable
add_subdirectory(src/app)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/unit)
endif()
