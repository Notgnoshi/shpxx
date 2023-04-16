#pragma once

#include "shpxx/io/openmode.hpp"
#include "shpxx/shpfile.hpp"

#include <string>

namespace shpxx::io::file {
//! @brief Open a shape file
//!
//! This will open not only the root .shp file, but other associated files that
//! can be found next to it.
//!
//! @param[in] file_path Path to the .shp file to open
//! @param[in] mode Mode to open the file in
//!
//! @return A shpfile_t handle to the file
//!
//! @throw shpxx::invalid_file_error_t If opening the file fails
shpxx::shpfile_t open_shp(const std::string& file_path, shpxx::io::openmode mode);
}  // namespace shpxx::io::file
