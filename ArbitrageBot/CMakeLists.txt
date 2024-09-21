cmake_minimum_required(VERSION 3.16)
project(ArbitrageBot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(configured)

option(ENABLE_TESTS "Enable building of tests" OFF)

if (ENABLE_TESTS)
    add_subdirectory(test)
