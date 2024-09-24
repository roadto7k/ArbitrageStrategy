cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(Network)

if (NOT TARGET CommonLib)
    add_subdirectory(../Common CommonLib)
endif()

if (NOT TARGET ConfiguredModule)
    add_subdirectory(../Configured Configured)
endif()


if (WIN32)
    set(VCKPG_CMAKE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake")
    message(STATUS ".....................................")
    message(STATUS "${VCPKG_DIR}")
    set(CMAKE_TOOLCHAIN_FILE ${VCKPG_CMAKE} CACHE STRING "VCPKG toolchain file")
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows-static")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")

find_package(Boost REQUIRED COMPONENTS filesystem system thread regex random)


FetchContent_Declare(
    websocketpp
    GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
    GIT_TAG master
)
FetchContent_MakeAvailable(websocketpp)

# Chemin vers le dossier de source de websocketpp
set(WEBSOCKETPP_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/websocketpp-src")

# Configuration de websocketpp
execute_process(
    COMMAND ${CMAKE_COMMAND} .
    WORKING_DIRECTORY ${WEBSOCKETPP_SOURCE_DIR}
)

# Construction de websocketpp
execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${WEBSOCKETPP_SOURCE_DIR}
)

# Installation de websocketpp
execute_process(
    COMMAND ${CMAKE_COMMAND} --install .
    WORKING_DIRECTORY ${WEBSOCKETPP_SOURCE_DIR}
)

# if (NOT WEBSOCKETPP_INSTALL_RESULT EQUAL 0)
#     message(FATAL_ERROR "L'installation de websocketpp a échoué.")
# endif()

add_library(NetworkLib STATIC
            NetworkManager.cpp
            CurlHttpClient.cpp
            WebSocketClientWebSocketPP.cpp)
set(boost_dir "${Boost_INCLUDE_DIRS}/boost")
target_include_directories(NetworkLib PUBLIC ${boost_dir})
target_link_libraries(NetworkLib PUBLIC Boost::system Boost::thread Boost::regex Boost::random)
set(WEBSOCKETPP_INCLUDE_DIR "${websocketpp_SOURCE_DIR}/install/include")
target_include_directories(NetworkLib PUBLIC ${WEBSOCKETPP_INCLUDE_DIR})
target_include_directories(NetworkLib PRIVATE ${websocketpp_SOURCE_DIR}install/include/websocketpp)
message(STATUS "---------------------------------")
message(STATUS ${WEBSOCKETPP_INCLUDE_DIR})
message(STATUS ${boost_dir})
if (WIN32)
    target_include_directories(NetworkLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}\\..\\..\\include\\Network)
else()
    target_include_directories(NetworkLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include/Network)
endif()
find_package(CURL REQUIRED)

target_link_libraries(NetworkLib PRIVATE CURL::libcurl)




add_executable(TEST main.cpp)
target_link_libraries(TEST NetworkLib)