#pragma once

#define SHPXX_STRINGIFY_(value) #value
#define SHPXX_STRINGIFY(value) SHPXX_STRINGIFY_(value)

#ifndef SHPXX_TEST_DATA_PATH
    #error "Missing absolute file path to test data directory"
#else
// #pragma message "Test data file path: " SHPXX_STRINGIFY(SHPXX_TEST_DATA_PATH)
#endif
#include <string>

namespace shpxx::testing {
static constexpr const std::string_view data_directory = SHPXX_STRINGIFY(SHPXX_TEST_DATA_PATH);
}  // namespace shpxx::testing

#undef SHPXX_STRINGIFY_
#undef SHPXX_STRINGIFY
#undef SHPXX_TEST_DATA_PATH
