cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(ExchangeAPI)

if (NOT TARGET CommonLib)
    add_subdirectory(../Common CommonLib)
endif()

if (NOT TARGET ConfiguredModule)
    add_subdirectory(../Configured Configured)
endif()

if (NOT TARGET NetworkLib)
    add_subdirectory(../Network NetworkLib)
endif()

# if (NOT TARGET MarketDataLib)
#     add_subdirectory(../MarketData MarketDataLib)
# endif()

add_library(ExchangeAPILib STATIC
            BinanceAPI.cpp
            )

target_include_directories(ExchangeAPILib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include/ExchangeAPI)
target_link_libraries(ExchangeAPILib NetworkLib UtilsLib nlohmann_json::nlohmann_json)

add_executable(TEST_EXCHANGE main.cpp)

target_link_libraries(TEST_EXCHANGE ExchangeAPILib)