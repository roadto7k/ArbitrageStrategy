cmake_minimum_required(VERSION 3.20)
project(ArbitrageBot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(configured)

option(ENABLE_TESTS "Enable building of tests" OFF)

if (WIN32)
    message(STATUS "Compiling on Windows")

    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "VCPKG toolchain file")
    set(VCPKG_DIR "${CMAKE_SOURCE_DIR}/vcpkg")

    if (NOT EXISTS ${VCPKG_DIR})
        message(STATUS "vcpkg non trouvé, téléchargement et installation en cours...")

        execute_process(
            COMMAND git clone https://github.com/microsoft/vcpkg.git ${VCPKG_DIR}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE GIT_RESULT
        )
        if (NOT GIT_RESULT EQUAL 0)
            message(FATAL_ERROR "Le clonage de vcpkg a échoué.")
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} -E chdir ${VCPKG_DIR} .\\bootstrap-vcpkg.bat
            WORKING_DIRECTORY ${VCPKG_DIR}
            RESULT_VARIABLE VCPKG_BOOTSTRAP_RESULT
        )
        if (NOT VCPKG_BOOTSTRAP_RESULT EQUAL 0)
            message(FATAL_ERROR "La configuration de vcpkg a échoué.")
        endif()
    endif()
    # set(CMAKE_PREFIX_PATH "${VCPKG_DIR}/installed/x64-windows-static" CACHE STRING "Path to static vcpkg installed libraries")
    # set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "VCPKG toolchain file")

    execute_process(
        COMMAND ${VCPKG_DIR}/vcpkg install curl
        RESULT_VARIABLE VCPKG_CURL_RESULT
    )

    execute_process(
        COMMAND ${VCPKG_DIR}/vcpkg install openssl
        RESULT_VARIABLE VCPKG_OpenSSL_RESULT
    )
    if (NOT VCPKG_CURL_RESULT EQUAL 0)
        message(FATAL_ERROR "L'installation de libcurl via vcpkg a échoué.")
    endif()

    if (NOT EXISTS "${VCPKG_DIR}/installed/x64-windows/include/boost")
        message(STATUS "Installation de Boost...")
        execute_process(
            COMMAND ${VCPKG_DIR}/vcpkg install boost:x64-windows
            RESULT_VARIABLE VCPKG_BOOST_RESULT
        )
        if (NOT VCPKG_BOOST_RESULT EQUAL 0)
            message(FATAL_ERROR "L'installation de libboost via vcpkg a échoué.")
        endif()
    else()
        message(STATUS "Boost est déjà installé.")
    endif()

    set(CMAKE_PREFIX_PATH "${VCPKG_DIR}/installed/x64-windows-static" CACHE STRING "Path to static vcpkg installed libraries")
    set(CMAKE_PREFIX_PATH "${VCPKG_DIR}/installed/x64-windows" CACHE STRING "Path to vcpkg installed libraries")
    # set(CMAKE_PREFIX_PATH "${VCPKG_DIR}/installed/x64-windows/debug" CACHE STRING "Path to vcpkg installed debug libraries")

endif()

# set(CURL_STATICLIB ON)

# find_package(CURL REQUIRED)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)


FetchContent_MakeAvailable(json)

add_subdirectory(src)
if (ENABLE_TESTS)
    add_subdirectory(test)
endif()

if (WIN32)
    message(STATUS "Compiling on Windows")
elseif(UNIX)
    message(STATUS "Compiling on Linux")
endif()