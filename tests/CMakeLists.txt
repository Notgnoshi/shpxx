add_executable(shpxx-tests)

find_package(GTest REQUIRED)

file(
    GLOB_RECURSE
    SHPXX_TEST_SOURCES
    CONFIGURE_DEPENDS
    *.cpp
)

include(ThirdPartyTargetLinkLibraries)

target_sources(shpxx-tests PRIVATE "${SHPXX_TEST_SOURCES}")
target_include_directories(shpxx-tests PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(shpxx-tests PRIVATE shpxx)
third_party_target_link_libraries(shpxx-tests PRIVATE GTest::gtest_main)
target_compile_features(shpxx-tests PRIVATE cxx_std_20)

# Add 'ALL' to add_custom_target arguments below if autorun mode is enabled (otherwise the
# CHECK_ALL_ARG variable defaults to empty string)
if(SHPXX_AUTORUN_TESTS)
    set(CHECK_ALL_ARG ALL)
endif()

add_custom_target(
    run-shpxx-tests
    ${CHECK_ALL_ARG}
    COMMAND "$<TARGET_FILE:shpxx-tests>" --gtest_output=xml:"${SHPXX_REPORTS_DIR}/shpxx-tests.xml"
    BYPRODUCTS "${SHPXX_REPORTS_DIR}/shpxx-tests.xml"
    COMMENT "Running unit tests..."
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:shpxx-tests>"
    USES_TERMINAL
)

# Generic target 'make check', but only if this is the root project and therefore these won't
# conflict with other projects
if(SHPXX_IS_ROOT_PROJECT)
    add_custom_target(check COMMENT "Run the unit tests")
    add_dependencies(check run-shpxx-tests)
endif()
