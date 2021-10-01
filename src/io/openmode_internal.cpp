#include "openmode_internal.hpp"

#include "shpxx/io/openmode.hpp"

#include <string_view>

namespace shpxx::io {

namespace {
    bool has_mode(std::string_view mode, std::string_view target)
    {
        return mode.find(target) != std::string_view::npos;
    }
}  // namespace

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
