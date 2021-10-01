#pragma once

#include "openmode.hpp"

#include <string>

namespace shpxx::io::file {
void open_shp(const std::string& file_path, shpxx::io::openmode mode);
}
