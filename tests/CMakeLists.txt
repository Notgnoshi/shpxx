add_executable(shpxx-tests)

find_package(GTest REQUIRED)

file(
    GLOB_RECURSE
    SHPXX_TEST_SOURCES
    CONFIGURE_DEPENDS
    *.cpp
)

target_sources(shpxx-tests PRIVATE ${SHPXX_TEST_SOURCES})
target_link_libraries(shpxx-tests GTest::gtest_main)

# Add 'ALL' to add_custom_target arguments below if autorun mode is enabled (otherwise the
# CHECK_ALL_ARG variable defaults to empty string)
if(SHPXX_AUTORUN_TESTS)
    set(CHECK_ALL_ARG ALL)
endif()

add_custom_target(
    run-shpxx-tests
    ${CHECK_ALL_ARG}
    COMMAND $<TARGET_FILE:shpxx-tests> --gtest_output=xml:${SHPXX_REPORTS_DIR}/shpxx-tests.xml
    BYPRODUCTS ${SHPXX_REPORTS_DIR}/shpxx-tests.xml
    COMMENT "Running unit tests..."
    USES_TERMINAL
)
