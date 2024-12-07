cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(Strategy)

if (NOT TARGET CommonLib)
    add_subdirectory(../Common CommonLib)
endif()

if (NOT TARGET ConfiguredModule)
    add_subdirectory(../Configured Configured)
endif()

add_library(StrategyLib INTERFACE)
target_link_libraries(StrategyLib INTERFACE UtilsLib)
target_include_directories(StrategyLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../include/Strategy)

add_executable(TEST_STRAT main_strat.cpp)
target_link_libraries(TEST_STRAT PRIVATE StrategyLib)

add_executable(TEST_GENERATOR main_generator.cpp)
target_link_libraries(TEST_GENERATOR PRIVATE StrategyLib)

add_executable(TEST_WALLET main_wallet.cpp)
target_link_libraries(TEST_WALLET PRIVATE StrategyLib)