#include "openmode_internal.hpp"

#include "shpxx/io/openmode.hpp"

#include <string_view>

namespace shpxx::io {

namespace {
    bool has_mode(openmode mode, openmode target) { return (mode & target) != 0; }

    bool has_mode(std::string_view mode, std::string_view target)
    {
        return mode.find(target) != std::string_view::npos;
    }
}  // namespace

std::string to_string(openmode mode)
{
    std::string mode_string;
    if (has_mode(mode, io::read_only))
    {
        mode_string.push_back('r');
    }

    mode_string.push_back('b');
    return mode_string;
}

std::ios::openmode to_openmode(std::string_view mode)
{
    // We always use binary mode, no matter what
    std::ios::openmode std_mode = std::ios::binary;
    if (has_mode(mode, "r"))
    {
        std_mode |= std::ios::in;
    }

    return std_mode;
}

}  // namespace shpxx::io
