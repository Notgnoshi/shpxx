#pragma once

#include "shpxx/coordinate.hpp"

#include <concepts>

namespace shpxx::concepts {

template<class ObjectT>
concept IsCoordinate = std::same_as<ObjectT, shpxx::coordinate_xy_t> ||
                       std::same_as<ObjectT, shpxx::coordinate_xyz_t> ||
                       std::same_as<ObjectT, shpxx::coordinate_xym_t> ||
                       std::same_as<ObjectT, shpxx::coordinate_xyzm_t>;

}  // namespace shpxx::concepts
