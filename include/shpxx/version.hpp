#pragma once

#include <cstddef>
#include <string>

namespace shpxx {
struct version_t
{
    const static std::size_t major;
    const static std::size_t minor;
    const static std::size_t patch;

    const static std::string version_str;
    const static std::string git_hash;
    const static std::string build_timestamp;
};
}  // namespace shpxx
