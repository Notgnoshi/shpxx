cmake_policy(SET CMP0048 NEW)

project(
    shpxx
    VERSION 0
    DESCRIPTION "A modern C++ wrapper for shapelib"
    LANGUAGES CXX
)

cmake_minimum_required(VERSION 3.14)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(GTest REQUIRED)
find_package(shapelib REQUIRED)

if(NOT DEFINED SHPXX_IS_ROOT_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(SHPXX_IS_ROOT_PROJECT ON)
    else()
        set(SHPXX_IS_ROOT_PROJECT OFF)
    endif()
endif()
