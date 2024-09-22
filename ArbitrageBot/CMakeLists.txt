cmake_minimum_required(VERSION 3.16)
project(ArbitrageBot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(configured)

option(ENABLE_TESTS "Enable building of tests" OFF)

find_package(CURL REQUIRED)

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
)
FetchContent_MakeAvailable(json)

add_subdirectory(src)
if (ENABLE_TESTS)
    add_subdirectory(test)

if (WIN32)
    message(STATUS "Compiling on Windows")
elseif(UNIX)
    message(STATUS "Compiling on Linux")
endif()