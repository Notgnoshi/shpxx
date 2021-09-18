include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG "master"
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)

    # When building gtest along with this project, we want to always build gtest as a static
    # library, regardless of the BUILD_SHARED_LIBS variable
    set(OLD_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
    set(BUILD_SHARED_LIBS OFF)

    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

    set(BUILD_SHARED_LIBS ${OLD_BUILD_SHARED_LIBS})
endif()

add_library(GTest::gtest ALIAS gtest)
add_library(GTest::gtest_main ALIAS gtest_main)
add_library(GTest::gmock ALIAS gmock)
add_library(GTest::gmock_main ALIAS gmock_main)
