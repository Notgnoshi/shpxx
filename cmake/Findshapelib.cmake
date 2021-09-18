include(FetchContent)

FetchContent_Declare(
    shapelib
    GIT_REPOSITORY https://github.com/OSGeo/shapelib.git
    GIT_TAG "master"
    PATCH_COMMAND
        ${CMAKE_COMMAND} -DFILE=${CMAKE_CURRENT_BINARY_DIR}/_deps/shapelib-src/CMakeLists.txt -P
        ${CMAKE_CURRENT_LIST_DIR}/DisableBuildTest.cmake
)

FetchContent_GetProperties(shapelib)
if(NOT shapelib_POPULATED)
    FetchContent_Populate(shapelib)
    add_subdirectory(${shapelib_SOURCE_DIR} ${shapelib_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_library(shapelib::shp ALIAS shp)
