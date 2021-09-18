cmake_policy(SET CMP0048 NEW)

project(
    shpxx
    VERSION 0
    DESCRIPTION "A modern C++ wrapper for shapelib"
    LANGUAGES CXX
)

cmake_minimum_required(VERSION 3.14)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(shapelib REQUIRED)

if(NOT DEFINED SHPXX_IS_ROOT_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(SHPXX_IS_ROOT_PROJECT ON)
    else()
        set(SHPXX_IS_ROOT_PROJECT OFF)
    endif()
endif()

include(CMakeDependentOption)

option(SHPXX_BUILD_TESTS "Build library tests" ${SHPXX_IS_ROOT_PROJECT})
cmake_dependent_option(
    SHPXX_AUTORUN_TESTS
    "Auto-run tests after building"
    ${SHPXX_IS_ROOT_PROJECT}
    "SHPXX_BUILD_TESTS"
    OFF
)

if(SHPXX_BUILD_TESTS)
    add_subdirectory(tests)
endif()
