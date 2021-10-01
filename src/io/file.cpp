#include "shpxx/io/file.hpp"

#include "io/file_handler.hpp"
#include "io/safile_wrapper.hpp"

#include <shapefil.h>

namespace shpxx::io::file {
void open_shp(const std::string& file_path, shpxx::io::openmode mode)
{
    auto sa_hooks = make_sahooks<file_handler_t>();
    SHPOpenLL(file_path.c_str(), to_string(mode).c_str(), &sa_hooks);
}
}  // namespace shpxx::io::file
