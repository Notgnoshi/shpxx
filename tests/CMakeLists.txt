add_executable(shpxx-tests)

find_package(GTest REQUIRED)

file(
    GLOB_RECURSE
    TEST_SOURCES
    CONFIGURE_DEPENDS
    *.cpp
)

target_sources(shpxx-tests PRIVATE ${TEST_SOURCES})
target_link_libraries(shpxx-tests GTest::gtest_main)

# Add 'ALL' to add_custom_target arguments below if autorun mode is enabled (otherwise the
# CHECK_ALL_ARG variable defaults to empty string)
if(SHPXX_AUTORUN_TESTS)
    set(CHECK_ALL_ARG ALL)
endif()

add_custom_target(
    run-shpxx-tests
    ${CHECK_ALL_ARG}
    COMMAND $<TARGET_FILE:shpxx-tests>
    COMMENT "Running unit tests..."
    USES_TERMINAL
)
