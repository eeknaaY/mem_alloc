cmake_minimum_required(VERSION 3.20.0)

project(mem_alloc
    VERSION 0.1
    DESCRIPTION "Custom (Bad) Memory Allocator"
    LANGUAGES CXX
)

file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(mem_alloc
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_compile_features(mem_alloc PRIVATE cxx_std_20)

target_include_directories(mem_alloc PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)
