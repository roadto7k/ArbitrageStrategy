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

# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (WIN32)
    set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
endif()

find_package(Boost REQUIRED COMPONENTS filesystem system thread regex random)
find_package(OpenSSL REQUIRED)

FetchContent_Declare(
    websocketpp
    GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
    GIT_TAG develop
)
FetchContent_MakeAvailable(websocketpp)

if (WIN32)
    set(WEBSOCKETPP_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/websocketpp-src")


    execute_process(
        COMMAND ${CMAKE_COMMAND} .
        WORKING_DIRECTORY ${WEBSOCKETPP_SOURCE_DIR}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${WEBSOCKETPP_SOURCE_DIR}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} --install .
        WORKING_DIRECTORY ${WEBSOCKETPP_SOURCE_DIR}
    )
endif()
# if (NOT WEBSOCKETPP_INSTALL_RESULT EQUAL 0)
#     message(FATAL_ERROR "L'installation de websocketpp a échoué.")
# endif()

add_library(NetworkLib STATIC
            NetworkManager.cpp
            CurlHttpClient.cpp
            WebSocketClientWebSocketPP.cpp)
set(boost_dir "${Boost_INCLUDE_DIRS}/boost")
set(open_ssl_dir "${Boost_INCLUDE_DIRS}/openssl")
message(STATUS "BIIIIIIITE")
message(STATUS ${open_ssl_dir})

target_include_directories(NetworkLib PUBLIC ${boost_dir} ${open_ssl_dir})
target_link_directories(NetworkLib PUBLIC ${open_ssl_dir})
target_link_libraries(NetworkLib PUBLIC Boost::system Boost::thread Boost::regex Boost::random OpenSSL::SSL OpenSSL::Crypto)
set(WEBSOCKETPP_INCLUDE_DIR "${websocketpp_SOURCE_DIR}/install/include")
if (WIN32)
    set(WEBSOCKETPP_INCLUDE_DIR "${websocketpp_SOURCE_DIR}/install/include")
    target_include_directories(NetworkLib PRIVATE ${websocketpp_SOURCE_DIR}install/include/websocketpp)
else()
message(STATUS ${websocketpp_SOURCE_DIR})
message(STATUS "...")
    set(WEBSOCKETPP_INCLUDE_DIR "${websocketpp_SOURCE_DIR}")
    target_include_directories(NetworkLib PRIVATE ${websocketpp_SOURCE_DIR})
endif()
target_include_directories(NetworkLib PUBLIC ${WEBSOCKETPP_INCLUDE_DIR})
message(STATUS "---------------------------------")
message(STATUS ${WEBSOCKETPP_INCLUDE_DIR})
message(STATUS ${boost_dir})
if (WIN32)
    target_include_directories(NetworkLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}\\..\\..\\include\\Network)
else()
    target_include_directories(NetworkLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include/Network)
endif()
# set(CURL_STATICLIB ON)

# set(CURL_ROOT "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/debug")
# set(CURL_INCLUDEDIR "${CURL_ROOT}/include")
# set(CURL_LIBRARYDIR "${CURL_ROOT}/lib")



find_package(CURL REQUIRED)

message(STATUS "CURL include dirs: ${CURL_INCLUDE_DIRS}")
message(STATUS "CURL libraries: ${CURL_LIBRARIES}")
# set(CURL_STATICLIB ON)

# set(CURL_ROOT "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows-static")
# set(CURL_INCLUDEDIR "${CURL_ROOT}/include")
# set(CURL_LIBRARYDIR "${CURL_ROOT}/lib")
target_link_libraries(NetworkLib PUBLIC CURL::libcurl)
if (WIN32)
    set(CURL_HEADER_PATH "${VCPKG_DIR}/installed/x64-windows/include")
endif()

message(STATUS "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMHHHHHHHHHHHHHHH")
message(STATUS ${CURL_INCLUDE_DIRS})
message(STATUS ${CURL_LIBRARIES})
target_include_directories(NetworkLib PUBLIC ${CURL_HEADER_PATH})


add_executable(TEST main.cpp)
target_link_libraries(TEST NetworkLib)

if (WIN32 AND NOT CURL_STATICLIB)
    add_custom_command(TARGET TEST POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VCPKG_DIR}/installed/x64-windows/bin/libcurl-d.dll"
        $<TARGET_FILE_DIR:TEST>)
endif()

if (WIN32)
    add_custom_command(TARGET TEST POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VCPKG_DIR}/installed/x64-windows/debug/bin/boost_random-vc143-mt-gd-x64-1_85.dll"
        $<TARGET_FILE_DIR:TEST>)
endif()