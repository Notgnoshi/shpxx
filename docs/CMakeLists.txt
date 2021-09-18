# This subdir is conditionally added based on the SHPXX_BUILD_DOCS option. See
# https://www.doxygen.nl/manual/config.html for details on Doxygen configuration
find_package(Doxygen REQUIRED dot)

# Match declarations and definitions using STL classes
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
# Build docs for things that don't have documentation markup
set(DOXYGEN_EXTRACT_ALL YES)

# Build private/internal documentation, if enabled.
set(DOXYGEN_EXTRACT_PRIVATE ${SHPXX_BUILD_PRIVATE_DOCS})
set(DOXYGEN_EXTRACT_PRIV_VIRTUAL ${SHPXX_BUILD_PRIVATE_DOCS})
set(DOXYGEN_EXTRACT_STATIC ${SHPXX_BUILD_PRIVATE_DOCS})

# Add a timestamp to the footer of each generated HTML page
set(DOXYGEN_HTML_TIMESTAMP YES)
# Allow collapsing sections in the generated pages
set(DOXYGEN_HTML_DYNAMIC_SECTIONS YES)
# Provide a tree-like index
set(DOXYGEN_GENERATE_TREEVIEW YES)
# Enable zooming and panning in generated diagrams
set(DOXYGEN_DOT_IMAGE_FORMAT svg)
set(DOXYGEN_INTERACTIVE_SVG YES)
# Generate graphs of who calls what
set(DOXYGEN_CALLER_GRAPH YES)
set(DOXYGEN_CALL_GRAPH YES)
# Use the README as the documentation main page
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
set(DOXYGEN_EXAMPLE_PATH ${PROJECT_SOURCE_DIR}/examples)
set(DOXYGEN_IMAGE_PATH ${PROJECT_SOURCE_DIR}/docs/images)

doxygen_add_docs(
    shpxx-build-docs
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests
    ${PROJECT_SOURCE_DIR}/examples
    ${PROJECT_SOURCE_DIR}/README.md
    COMMENT "Generate Doxygen documentation for shpxx"
)
