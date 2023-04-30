#pragma once

#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"

namespace shpxx::feature {
class polygon_t
{
  public:
    explicit polygon_t(shplib::opaque_object_t);

    [[nodiscard]] static bool is_compatible(shape_type t) noexcept;
};
}  // namespace shpxx::feature
