# Allow specifying VERSION in project() statements
cmake_policy(SET CMP0048 NEW)

# Allow overriding option() in sub-projects by using set() before adding them
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

cmake_minimum_required(VERSION 3.14)

project(
    shpxx
    VERSION 0
    DESCRIPTION "A modern C++ wrapper for shapelib"
    LANGUAGES CXX
)

# Default BUILD_SHARED_LIBS to off so that subprojects don't get to be the first place this is
# defined
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(GTest REQUIRED)
find_package(shapelib REQUIRED)
