#include "shpxx/shp_handle.hpp"

#include "shp_casts.hpp"

#include <shapefil.h>

namespace shpxx::shplib {
void close_file_t::operator()(shplib::shp_info* p) const { SHPClose(cast_shp_info(p)); }

void destroy_object_t::operator()(shp_object* p) const { SHPDestroyObject(cast_shp_object(p)); }

}  // namespace shpxx::shplib
