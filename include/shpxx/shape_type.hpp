#pragma once

#include <string>

namespace shpxx {
//! @brief Enumeration of types of shapes that can be in a file
//!
//! The definitions of these enum values is defined in the ESRI shapefile
//! standard and should not be changed.
enum class shape_type
{
    null = 0,
    point = 1,
    linestring = 3,
    polygon = 5,
    multi_point = 8,
    point_z = 11,
    linestring_z = 13,
    polygon_z = 15,
    multi_point_z = 18,
    point_m = 21,
    linestring_m = 23,
    polygon_m = 25,
    multi_point_m = 28,
    multi_patch = 31
};

std::string to_string(shape_type type) noexcept;
}  // namespace shpxx
