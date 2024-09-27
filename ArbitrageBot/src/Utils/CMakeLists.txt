cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(Utils)

if (NOT TARGET CommonLib)
    add_subdirectory(../Common CommonLib)
endif()

if (NOT TARGET ConfiguredModule)
    add_subdirectory(../Configured Configured)
endif()


add_library(UtilsLib INTERFACE)

target_link_libraries(UtilsLib INTERFACE nlohmann_json::nlohmann_json)
target_include_directories(UtilsLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../include/Utils)
target_link_libraries(UtilsLib INTERFACE CommonLib)

if (NOT TARGET TEST_UTILS)
    add_executable(TEST_UTILS main.cpp)

    target_link_libraries(TEST_UTILS PRIVATE UtilsLib)
endif()