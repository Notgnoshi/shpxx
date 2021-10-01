add_library(shpxx)

find_package(shapelib REQUIRED)

file(
    GLOB_RECURSE
    SHPXX_SOURCES
    CONFIGURE_DEPENDS
    "*.cpp"
    "*.cxx"
    "*.c"
    "*.cc"
)

include(VersionVariables)
configure_file(version.cpp.in version.cpp @ONLY)

target_sources(shpxx PRIVATE "${SHPXX_SOURCES}" "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")
target_include_directories(
    shpxx
    PUBLIC "${PROJECT_SOURCE_DIR}/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/src"
)
target_compile_features(
    shpxx
    INTERFACE cxx_std_17
    PRIVATE cxx_std_20
)

include(ThirdPartyTargetLinkLibraries)
third_party_target_link_libraries(shpxx PUBLIC shapelib::shp)
