#pragma once

#include "shpxx/io/openmode.hpp"

#include <ios>
#include <string>
#include <string_view>

namespace shpxx::io {
std::ios::openmode to_openmode(std::string_view mode);
}  // namespace shpxx::io
