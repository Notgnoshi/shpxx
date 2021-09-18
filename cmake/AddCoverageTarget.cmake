find_package(gcovr 4.2 REQUIRED)

instrument_coverage(shpxx-tests)
instrument_coverage(shpxx)

file(MAKE_DIRECTORY "${SHPXX_REPORTS_DIR}/shpxx-coverage")

add_custom_target(
    shpxx-coverage
    COMMAND
        "${GCOVR_PATH}" --xml "${SHPXX_REPORTS_DIR}/shpxx-coverage.xml" --html-details
        "${SHPXX_REPORTS_DIR}/shpxx-coverage/shpxx-coverage.html" --gcov-executable
        "${GCOV_COMMAND}" --branches --exclude-unreachable-branches --exclude-throw-branches -s
        --filter "${PROJECT_SOURCE_DIR}/src" --filter "${PROJECT_SOURCE_DIR}/include"
    BYPRODUCTS "${SHPXX_REPORTS_DIR}/shpxx-coverage.xml"
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
)

# If root project, create coverage target alias
if(SHPXX_IS_ROOT_PROJECT)
    add_custom_target(coverage)
    add_dependencies(coverage shpxx-coverage)
endif()
