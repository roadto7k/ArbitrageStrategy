cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(MarketData)

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

add_library(MarketDataLib INTERFACE)

target_include_directories(MarketDataLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../include/MarketData)
target_link_libraries(MarketDataLib INTERFACE UtilsLib NetworkLib CommonLib ConfiguredModule)

add_executable(TEST_MARKETDATA main.cpp)

target_link_libraries(TEST_MARKETDATA PRIVATE MarketDataLib)