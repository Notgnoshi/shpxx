#include "shpxx/io/file.hpp"

#include "io/file_handler.hpp"
#include "io/safile_wrapper.hpp"
#include "shp_casts.hpp"
#include "shpxx/exceptions.hpp"

#include <shapefil.h>

namespace shpxx::io::file {
shpxx::shpfile_t open_shp(const std::string& file_path, shpxx::io::openmode mode)
{
    auto sa_hooks = make_sahooks<file_handler_t>();
    auto* file_handle = SHPOpenLL(file_path.c_str(), to_string(mode).c_str(), &sa_hooks);

    return shpxx::shpfile_t(shp_handle_t(cast_shp_info(file_handle)));
}
}  // namespace shpxx::io::file
