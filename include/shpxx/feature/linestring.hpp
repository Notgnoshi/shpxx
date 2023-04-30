#pragma once

#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"

namespace shpxx::feature {
class linestring_t
{
  public:
    explicit linestring_t(shplib::opaque_object_t);

    [[nodiscard]] static bool is_compatible(shape_type t) noexcept;
    [[nodiscard]] static std::string class_name() noexcept
    {
        return "shpxx::feature::linestring_t";
    }
};
}  // namespace shpxx::feature
