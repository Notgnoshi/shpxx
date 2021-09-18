include(FetchContent)

FetchContent_Declare(
    shapelib
    GIT_REPOSITORY https://github.com/OSGeo/shapelib.git
    GIT_TAG "master"
    SOURCE_DIR ${PROJECT_BINARY_DIR}/_deps/shapelib-src
    PATCH_COMMAND ${CMAKE_COMMAND} -DFILE=${PROJECT_BINARY_DIR}/_deps/shapelib-src/CMakeLists.txt
                  -P ${CMAKE_CURRENT_LIST_DIR}/DisableBuildTest.cmake
)

FetchContent_GetProperties(shapelib)
if(NOT shapelib_POPULATED)
    FetchContent_Populate(shapelib)

    # When building shapelib along with this project, we want to always build shapelib as a shared
    # library, regardless of the BUILD_SHARED_LIBS variable
    set(OLD_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
    set(BUILD_SHARED_LIBS ON)

    add_subdirectory(${shapelib_SOURCE_DIR} ${shapelib_BINARY_DIR} EXCLUDE_FROM_ALL)

    set(BUILD_SHARED_LIBS ${OLD_BUILD_SHARED_LIBS})
endif()

add_library(shapelib::shp ALIAS shp)
