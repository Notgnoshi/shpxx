#pragma once

#include <cmath>

namespace shpxx {
struct coordinate_xy_t
{
    double x;
    double y;
};

struct coordinate_xym_t
{
    double x;
    double y;
    double m;
};

struct coordinate_xyz_t
{
    double x;
    double y;
    double z;
};

struct coordinate_xyzm_t
{
    double x;
    double y;
    double z;
    double m;
};

//! @brief Per the ESRI shapefile specification, any Z, M, or otherwise "nullable" field
//! containing a value smaller than this is considered to contain no data.
constexpr static double no_data_threshold = 1E-38;

//! @brief Check if a value is "no data" by the definition in the ESRI standard
constexpr bool is_no_data(double value) noexcept { return std::abs(value) < no_data_threshold; }
}  // namespace shpxx
