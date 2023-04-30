#pragma once

#include "shpxx/concepts/is_coordinate.hpp"

namespace shpxx {
//! @brief Convert a coordinate to another type
//!
//! @note Any part of the original coordinate which is not present in the
//! destination type will be dropped. Any part which is present in the
//! destination type but not the original will be defaulted to 0.
//!
//! @tparam ToCoordT Type to convert to
//! @tparam FromCoordT Type to convert from
//!
//! @param from Value to convert from
//!
//! @return The converted value, with appropriately defaulted parts
template<concepts::IsCoordinate ToCoordT, concepts::IsCoordinate FromCoordT>
ToCoordT coordinate_cast(FromCoordT from);
}  // namespace shpxx

#include "shpxx/coordinate_cast_impl.hpp"
