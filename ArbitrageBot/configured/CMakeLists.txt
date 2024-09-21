if (NOT DEFINED PROJECT_ROOT)
    set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
endif()

add_library(ConfiguredModule STATIC dummy.cpp)

configure_file(
    config.hpp.in
    "${CMAKE_BINARY_DIR}/configured_files/include/config.hpp" ESCAPE_QUOTES
)

target_include_directories(ConfiguredModule INTERFACE "${CMAKE_BINARY_DIR}/configured_files/include")