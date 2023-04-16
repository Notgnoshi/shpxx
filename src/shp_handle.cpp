#include "shpxx/shp_handle.hpp"

#include "shp_casts.hpp"

#include <shapefil.h>

namespace shpxx::shp_handle {
void close_shp_t::operator()(shp_info* p) const { SHPClose(cast_shp_info(p)); }
}  // namespace shpxx::shp_handle
