add_library(shpxx)

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
target_include_directories(shpxx PUBLIC "${PROJECT_SOURCE_DIR}/include")
