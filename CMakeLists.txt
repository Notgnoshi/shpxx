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
