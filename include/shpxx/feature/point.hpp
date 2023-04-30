#pragma once

#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"

namespace shpxx::feature {
class point_t
{
  public:
    explicit point_t(shplib::opaque_object_t);

    [[nodiscard]] static bool is_compatible(shape_type t) noexcept;
    [[nodiscard]] static std::string class_name() noexcept { return "shpxx::feature::point_t"; }
};
}  // namespace shpxx::feature
