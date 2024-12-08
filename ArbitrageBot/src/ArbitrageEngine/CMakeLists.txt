cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT TARGET CommonLib)
    add_subdirectory(../Common CommonLib)
endif()

if (NOT TARGET ConfiguredModule)
    add_subdirectory(../Configured Configured)
endif()

if (NOT TARGET UtilsLib)
    add_subdirectory(../Utils UtilsLib)
endif()

if (NOT TARGET NetworkLib)
    add_subdirectory(../Network NetworkLib)
endif()

if (NOT TARGET ExchangeAPILib)
    add_subdirectory(../ExchangeAPI ExchangeAPILib)
endif()

if (NOT TARGET MarketDataLib)
    add_subdirectory(../MarketData MarketDataLib)
endif()

if (NOT TARGET StrategyLib)
    add_subdirectory(../Strategy StrategyLib)
endif()

project(ArbitrageEngine)

add_library(ArbitrageEngineLib INTERFACE)

target_include_directories(ArbitrageEngineLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../include/ArbitrageEngine)

target_link_libraries(ArbitrageEngineLib INTERFACE MarketDataLib StrategyLib ExchangeAPILib UtilsLib NetworkLib CommonLib ConfiguredModule)

add_executable(TEST_ARBITRAGE main.cpp)

target_link_libraries(TEST_ARBITRAGE ArbitrageEngineLib)

add_executable(TEST_V2ARBITRAGE main_trianglegen.cpp)

target_link_libraries(TEST_V2ARBITRAGE ArbitrageEngineLib)