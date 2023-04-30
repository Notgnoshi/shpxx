#pragma once

#include "shpxx/coordinate.hpp"

#include <shapefil.h>

namespace shpxx::shplib {
coordinate_xyzm_t get_min_bound(const SHPObject* obj);
coordinate_xyzm_t get_max_bound(const SHPObject* obj);
}  // namespace shpxx::shplib
