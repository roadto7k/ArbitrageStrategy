cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(Strategy)

add_library(StrategyLib INTERFACE)

target_include_directories(StrategyLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../include/Strategy)