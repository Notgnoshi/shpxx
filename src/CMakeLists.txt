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

target_sources(shpxx PRIVATE "${SHPXX_SOURCES}")
target_include_directories(shpxx PUBLIC "${PROJECT_SOURCE_DIR}/include")
