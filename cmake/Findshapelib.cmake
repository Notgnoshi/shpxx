include(FetchContent)

FetchContent_Declare(
    shapelib
    GIT_REPOSITORY https://github.com/OSGeo/shapelib.git
    GIT_TAG "v1.6.3"
    SOURCE_DIR ${PROJECT_BINARY_DIR}/_deps/shapelib-src
)

FetchContent_GetProperties(shapelib)
if(NOT shapelib_POPULATED)
    FetchContent_Populate(shapelib)

    # When building shapelib along with this project, we want to always build shapelib as a shared
    # library, regardless of the BUILD_SHARED_LIBS variable
    set(OLD_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
    set(OLD_BUILD_TESTING ${BUILD_TESTING})
    set(BUILD_SHARED_LIBS ON)
    set(BUILD_TESTING OFF)

    add_subdirectory(${shapelib_SOURCE_DIR} ${shapelib_BINARY_DIR} EXCLUDE_FROM_ALL)
    target_include_directories(shp INTERFACE $<BUILD_INTERFACE:${shapelib_SOURCE_DIR}>)

    set(BUILD_SHARED_LIBS ${OLD_BUILD_SHARED_LIBS})
    set(BUILD_TESTING ${OLD_BUILD_TESTING})
endif()

add_library(shapelib::shp ALIAS shp)
