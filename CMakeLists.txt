# Allow specifying VERSION in project() statements
cmake_policy(SET CMP0048 NEW)

# Allow overriding option() in sub-projects by using set() before adding them
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

cmake_minimum_required(VERSION 3.14)

project(
    shpxx
    VERSION 0.0.0
    DESCRIPTION "A modern C++ wrapper for shapelib"
    LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default BUILD_SHARED_LIBS to off so that subprojects don't get to be the first place this is
# defined
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

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
cmake_dependent_option(
    SHPXX_INSTRUMENT_COVERAGE
    "Instrument the project to generate a coverage report. For best result, couple this with -DCMAKE_BUILD_TYPE=Debug"
    OFF
    "SHPXX_BUILD_TESTS"
    OFF
)

option(SHPXX_BUILD_DOCS "Build library documentation" ${SHPXX_IS_ROOT_PROJECT})
cmake_dependent_option(
    SHPXX_BUILD_PRIVATE_DOCS
    "Build documentation for private/internal members"
    ${SHPXX_IS_ROOT_PROJECT}
    "SHPXX_BUILD_DOCS"
    OFF
)

option(SHPXX_BUILD_EXAMPLES "Build project examples" ${SHPXX_IS_ROOT_PROJECT})

# Make a directory for outputting reports to
set(SHPXX_REPORTS_DIR "${CMAKE_BINARY_DIR}/reports")
file(MAKE_DIRECTORY "${SHPXX_REPORTS_DIR}")

add_subdirectory(src)

if(SHPXX_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(SHPXX_INSTRUMENT_COVERAGE)
    include(AddCoverageTarget)
endif()

if(SHPXX_BUILD_DOCS)
    add_subdirectory(docs)
endif()

if(SHPXX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
