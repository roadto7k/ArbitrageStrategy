cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Common)

add_library(CommonLib STATIC 
    Logger.cpp
    )

target_include_directories(CommonLib PUBLIC "../../include/Common")
# set_target_properties(CommonLib PROPERTIES PUBLIC_HEADER "../../../include/Common/Logger.h")

target_include_directories(CommonLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})