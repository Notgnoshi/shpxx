#include "shpxx/shape_type.hpp"

#include <string>

namespace shpxx {

std::string to_string(shape_type type) noexcept
{
    switch (type)
    {
    case shape_type::null:
        return "null";
    case shape_type::point:
        return "point_xy";
    case shape_type::linestring:
        return "linestring_xy";
    case shape_type::polygon:
        return "polygon_xy";
    case shape_type::multi_point:
        return "multi_point_xy";
    case shape_type::point_z:
        return "point_xyzm";
    case shape_type::linestring_z:
        return "linestring_xyzm";
    case shape_type::polygon_z:
        return "polygon_xyzm";
    case shape_type::multi_point_z:
        return "multi_point_xyzm";
    case shape_type::point_m:
        return "point_xym";
    case shape_type::linestring_m:
        return "linestring_xym";
    case shape_type::polygon_m:
        return "polygon_xym";
    case shape_type::multi_point_m:
        return "multi_point_xym";
    case shape_type::multi_patch:
        return "multi_patch";
    }

    return "unknown(" + std::to_string(static_cast<int>(type)) + ")";
}
}  // namespace shpxx
